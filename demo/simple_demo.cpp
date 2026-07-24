/**
 * v192_decoder DLL 简单使用示例
 * 
 * 编译方法：
 *   使用 CMake: 参考 demo/CMakeLists.txt
 *   或手动编译: cl /EHsc /std:c++17 /I"..\include" simple_demo.cpp /link "..\build\Release\v192_decoder.lib"
 * 
 * 运行前确保 v192_decoder.dll 在可执行文件目录或 PATH 中
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <v192_decoder.h>

// 全局统计信息
typedef struct {
    int total_frames;
    int total_points;
    double first_timestamp;
    double last_timestamp;
} Statistics;

Statistics g_stats = {0, 0, 0.0, 0.0};

/**
 * 帧回调函数
 * 每当解码器完成一帧点云时，会调用此函数
 */
void on_frame_ready(const V192Frame* frame, void* user_data) {
    g_stats.total_frames++;
    g_stats.total_points += frame->count;
    
    if (g_stats.total_frames == 1) {
        g_stats.first_timestamp = frame->timestamp;
    }
    g_stats.last_timestamp = frame->timestamp;
    
    // 打印帧信息
    printf("\n[Frame %5u] points=%6u  timestamp=%.3f  dual_return=%s\n",
           frame->frame_id, frame->count, frame->timestamp,
           frame->is_dual ? "yes" : "no");
    
    // 打印前 5 个点的详细信息
    unsigned int num_to_print = (frame->count < 5) ? frame->count : 5;
    for (unsigned int i = 0; i < num_to_print; i++) {
        const V192Point* pt = &frame->points[i];
        printf("  [%2u] dist=%6.3fm  az=%7.2f°  el=%7.2f°  "
               "xyz=(%7.3f, %7.3f, %7.3f)  refl=%3d  ring=%2d\n",
               i, pt->distance, pt->azimuth, pt->elevation,
               pt->x, pt->y, pt->z, pt->reflectivity, pt->ring);
    }
    
    // 每 10 帧打印一次统计信息
    if (g_stats.total_frames % 10 == 0) {
        double duration = g_stats.last_timestamp - g_stats.first_timestamp;
        double avg_points = (double)g_stats.total_points / g_stats.total_frames;
        
        printf("\n--- Statistics ---\n");
        printf("  Total frames: %d\n", g_stats.total_frames);
        printf("  Total points: %d\n", g_stats.total_points);
        printf("  Avg points/frame: %.0f\n", avg_points);
        if (duration > 0) {
            printf("  Duration: %.2f seconds\n", duration);
            printf("  Frame rate: %.1f fps\n", g_stats.total_frames / duration);
        }
        printf("------------------\n\n");
    }
}

/**
 * 打印使用说明
 */
void print_usage(const char* program_name) {
    printf("\nv192_decoder DLL 使用示例\n\n");
    printf("用法:\n");
    printf("  %s online [port]           - 从网卡在线捕获 (默认端口 8899)\n", program_name);
    printf("  %s pcap <file.pcap>        - 从 PCAP 文件离线解析\n", program_name);
    printf("\n示例:\n");
    printf("  %s online                  - 在线捕获，使用默认端口 8899\n", program_name);
    printf("  %s online 2368             - 在线捕获，使用端口 2368\n", program_name);
    printf("  %s pcap data.pcap          - 解析 PCAP 文件\n", program_name);
    printf("\n注意:\n");
    printf("  - 在线模式需要管理员权限\n");
    printf("  - 确保已安装 Npcap (https://npcap.com/)\n");
    printf("  - 按 Ctrl+C 停止在线捕获\n\n");
}

/**
 * 主函数
 */
int main(int argc, char* argv[]) {
    // 检查参数
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* mode = argv[1];
    
    // 创建解码器实例（使用内置校准数据）
    V192DecoderHandle decoder = v192_create(NULL);
    if (!decoder) {
        fprintf(stderr, "错误: 无法创建解码器实例\n");
        return 1;
    }
    
    // 设置回调函数
    v192_set_frame_callback(decoder, on_frame_ready, NULL);
    
    // 根据模式启动解码器
    if (strcmp(mode, "online") == 0) {
        // 在线模式：从网卡捕获
        int port = 8899;  // 默认端口
        if (argc >= 3) {
            port = atoi(argv[2]);
        }
        
        printf("启动在线捕获模式...\n");
        printf("端口: %d\n", port);
        printf("设备: 自动选择\n");
        printf("\n等待数据... (按 Ctrl+C 停止)\n\n");
        
        if (v192_start_online(decoder, (uint16_t)port) != 0) {
            fprintf(stderr, "\n错误: 无法启动在线捕获\n");
            fprintf(stderr, "请检查:\n");
            fprintf(stderr, "  1. 是否以管理员权限运行\n");
            fprintf(stderr, "  2. 是否已安装 Npcap\n");
            fprintf(stderr, "  3. 雷达是否已连接并发送数据\n");
            v192_destroy(decoder);
            return 1;
        }
        
        // 在线模式会在后台线程运行，这里等待用户中断
        printf("解码器正在运行... 按 Ctrl+C 停止\n");
        
        // 简单的等待循环（实际应用中应该使用信号处理）
        while (1) {
            #ifdef _WIN32
            Sleep(1000);  // Windows
            #else
            sleep(1);     // Unix/Linux
            #endif
        }
        
        v192_stop(decoder);
        
    } else if (strcmp(mode, "pcap") == 0) {
        // 离线模式：从 PCAP 文件读取
        if (argc < 3) {
            fprintf(stderr, "错误: 请指定 PCAP 文件路径\n");
            print_usage(argv[0]);
            v192_destroy(decoder);
            return 1;
        }
        
        const char* pcap_file = argv[2];
        
        printf("启动离线解析模式...\n");
        printf("文件: %s\n\n", pcap_file);
        
        if (v192_open_pcap(decoder, pcap_file) != 0) {
            fprintf(stderr, "\n错误: 无法打开 PCAP 文件: %s\n", pcap_file);
            fprintf(stderr, "请检查:\n");
            fprintf(stderr, "  1. 文件路径是否正确\n");
            fprintf(stderr, "  2. 文件格式是否为标准 PCAP\n");
            fprintf(stderr, "  3. 是否有读取权限\n");
            v192_destroy(decoder);
            return 1;
        }
        
        // 逐帧读取直到文件结束
        int result;
        while ((result = v192_read_next_frame(decoder)) == 0) {
            // 回调函数会在 v192_read_next_frame 内部被调用
        }
        
        if (result < 0) {
            fprintf(stderr, "\n错误: 读取 PCAP 文件时发生错误\n");
        } else {
            printf("\n解析完成!\n");
        }
        
        v192_close_pcap(decoder);
        
    } else {
        fprintf(stderr, "错误: 未知模式 '%s'\n", mode);
        print_usage(argv[0]);
        v192_destroy(decoder);
        return 1;
    }
    
    // 打印最终统计信息
    printf("\n=== 最终统计 ===\n");
    printf("总帧数: %d\n", g_stats.total_frames);
    printf("总点数: %d\n", g_stats.total_points);
    if (g_stats.total_frames > 0) {
        double avg_points = (double)g_stats.total_points / g_stats.total_frames;
        printf("平均点数/帧: %.0f\n", avg_points);
        
        double duration = g_stats.last_timestamp - g_stats.first_timestamp;
        if (duration > 0) {
            printf("总时长: %.2f 秒\n", duration);
            printf("平均帧率: %.1f fps\n", g_stats.total_frames / duration);
        }
    }
    printf("================\n\n");
    
    // 清理
    v192_destroy(decoder);
    
    return 0;
}
