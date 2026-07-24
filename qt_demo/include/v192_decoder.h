#pragma once

#include <stdint.h>
#include <stddef.h>

/* DLL export/import macros for Windows */
#ifdef _WIN32
    #ifdef V192_DECODER_EXPORTS
        #define V192_API __declspec(dllexport)
    #else
        #define V192_API __declspec(dllimport)
    #endif
#else
    #define V192_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  Data types                                                          */
/* ------------------------------------------------------------------ */

/** Single decoded point */
typedef struct {
    float    x;            /* Cartesian X (m), right-hand: X=forward */
    float    y;            /* Cartesian Y (m) */
    float    z;            /* Cartesian Z (m) */
    float    distance;     /* Radial distance (m) */
    float    azimuth;      /* Horizontal angle (degrees) */
    float    elevation;    /* Vertical angle (degrees) */
    uint8_t  reflectivity; /* Reflectivity / intensity 0-255 */
    uint8_t  ring;         /* Channel index 0-191 */
    double   timestamp;    /* Host timestamp (seconds since epoch) */
} V192Point;

/** One complete frame (one revolution) */
typedef struct {
    V192Point* points;     /* Pointer to point array (owned by library) */
    uint32_t   count;      /* Number of valid points in this frame */
    uint32_t   frame_id;   /* Frame sequence number from lidar */
    double     timestamp;  /* Timestamp of the last packet in this frame */
    int        is_dual;    /* Non-zero if dual-echo data is present */
} V192Frame;

/** Opaque decoder handle */
typedef void* V192DecoderHandle;

/** Frame-ready callback.
 *  Called from the decoder thread (online) or the caller's thread (pcap/feed).
 *  The frame pointer is valid only for the duration of the callback.
 *  @param frame     Pointer to the decoded frame.
 *  @param user_data User-supplied context pointer.
 */
typedef void (*V192FrameCallback)(const V192Frame* frame, void* user_data);

/* ------------------------------------------------------------------ */
/*  Lifecycle                                                           */
/* ------------------------------------------------------------------ */

/**
 * Create a decoder instance.
 * @param calib_file  Path to V192-Correction.json.
 *                    Pass NULL or "" to use built-in default angles.
 * @return Handle on success, NULL on failure.
 */
V192_API V192DecoderHandle v192_create(const char* calib_file);

/** Destroy a decoder instance and free all resources. */
V192_API void v192_destroy(V192DecoderHandle h);

/* ------------------------------------------------------------------ */
/*  Callback registration                                               */
/* ------------------------------------------------------------------ */

/** Register the frame-ready callback. Must be called before start/open. */
V192_API void v192_set_frame_callback(V192DecoderHandle h,
                                      V192FrameCallback  cb,
                                      void*              user_data);

/* ------------------------------------------------------------------ */
/*  Online mode (live UDP)                                              */
/* ------------------------------------------------------------------ */

/**
 * Start receiving packets from a live lidar.
 * Spawns an internal receive thread.
 * @param msop_port  UDP port for MSOP data packets (default 8899).
 * @return 0 on success, -1 on error.
 */
V192_API int  v192_start_online(V192DecoderHandle h, uint16_t msop_port);

/** Stop the receive thread started by v192_start_online(). */
V192_API void v192_stop(V192DecoderHandle h);

/* ------------------------------------------------------------------ */
/*  PCAP mode                                                           */
/* ------------------------------------------------------------------ */

/**
 * Open a pcap file for offline decoding.
 * @param pcap_file  Path to the .pcap file.
 * @return 0 on success, -1 on error.
 */
V192_API int v192_open_pcap(V192DecoderHandle h, const char* pcap_file);

/**
 * Decode packets until the next complete frame is ready, then invoke
 * the registered callback.
 * @return  0  A frame was decoded and the callback was invoked.
 *          1  End of file reached.
 *         -1  Error.
 */
V192_API int v192_read_next_frame(V192DecoderHandle h);

/**
 * Close the pcap file opened by v192_open_pcap().
 * Safe to call even if no file is open.
 */
V192_API void v192_close_pcap(V192DecoderHandle h);

/* ------------------------------------------------------------------ */
/*  Manual packet feed (integration mode)                              */
/* ------------------------------------------------------------------ */

/**
 * Feed a raw UDP payload into the decoder.
 * When a complete frame is assembled the callback is invoked synchronously.
 * @param data  Pointer to the raw packet bytes.
 * @param len   Length of the packet in bytes.
 * @return 1 if a new frame was completed, 0 otherwise, -1 on error.
 */
V192_API int v192_feed_packet(V192DecoderHandle h, const uint8_t* data, size_t len);

/* ------------------------------------------------------------------ */
/*  Configuration helpers                                               */
/* ------------------------------------------------------------------ */

/**
 * Set distance filter range.  Points outside [min_m, max_m] are discarded.
 * Call before start/open.  Defaults: min=0.0, max=300.0.
 */
V192_API void v192_set_distance_range(V192DecoderHandle h, float min_m, float max_m);

#ifdef __cplusplus
}
#endif
