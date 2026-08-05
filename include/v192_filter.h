#pragma once

#include "v192_decoder.h"
#include <vector>

#ifdef __cplusplus

/**
 * Parameters for the point-cloud neighbour filter.
 */
struct V192FilterParam {
    std::vector<double>              dis_seg          = {3000.0, 8000.0, 50000.0};
    std::vector<std::vector<double>> thres_func_param = {
        {3000.0,  0.0, 1000.0, 3.0, 130.0},
        {3000.0, 37.0, 1000.0, 2.0, 130.0},
        {3000.0, 37.0, 1000.0, 2.0, 130.0}
    };

    bool         smooth_flag      = false;
    int          check_neibor_num = 1;
    int          neibor_interval  = 1;
    unsigned int valid_num_thres  = 0;
    unsigned int close_dis_thres  = 50;   /* cm */

    bool         inter_process_flag = true;
    unsigned int inter_dis_thres    = 500; /* cm */

    bool  planar_assist_filter_flag = true;
    float planar_assist_thres       = 0.05f;
};

/**
 * Point-cloud neighbour filter for V192 frames.
 */
class V192_API V192PointCloudFilter {
public:
    explicit V192PointCloudFilter(const V192FilterParam& params = V192FilterParam{});
    ~V192PointCloudFilter();

    /** Hook this filter into the decoder pipeline. */
    void install(V192DecoderHandle h);

    /** Remove this filter from the decoder pipeline. */
    void uninstall(V192DecoderHandle h);

    /** Replace filter parameters at runtime. */
    void setParams(const V192FilterParam& params);

private:
    struct Impl;
    Impl* impl_;
};

#endif /* __cplusplus */
