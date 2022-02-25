//
// Created by Jia on 2022/2/25.
//
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libavutil/log.h"
#include "libavdevice/avdevice.h"
#include "x264.h"
}

#include <fstream>
#include <iostream>
#include "FFmpeg.h"

char av_error[AV_ERROR_MAX_STRING_SIZE] = {0};
#define av_err2str(errnum) av_make_error_string(av_error, AV_ERROR_MAX_STRING_SIZE, errnum)

int FFmpeg::test_h264() {
    x264_param_t param;
    x264_param_default(&param);
    av_log_set_level(AV_LOG_TRACE);
    auto codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (codec) {
        std::cout << "success!!!" << codec->name << std::endl;
    } else {
        std::cout << "failed!!!" << std::endl;
    }
    return 0;
}

void FFmpeg::record_video() {
    using namespace std;
    avdevice_register_all();
    const auto DEVICE = "video=Logitech HD Webcam C270";
    auto *fmt = av_find_input_format("dshow");
    AVDictionary *options = nullptr;
    av_dict_set(&options, "video_size", "640x360", 0);
    av_dict_set(&options, "pixel_format", "yuv420p", 0);
    av_dict_set(&options, "framerate", "30", 0);
    AVFormatContext *ctx = nullptr;
    auto res = avformat_open_input(&ctx, DEVICE, fmt, &options);
    if (res < 0) {
        av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
        return;
    }
    auto params = ctx->streams[0]->codecpar;
    auto bufferSize = av_image_get_buffer_size((AVPixelFormat) (params->format), params->width, params->height, 1);
    std::ofstream file("./record_video.yuv");
    auto pkt = av_packet_alloc();
    auto count = 60;
    while (true) {
        auto ret = av_read_frame(ctx, pkt);
        if (ret == 0) {
            file.write((const char *) pkt->data, bufferSize);
            av_packet_unref(pkt);
        } else if (ret == AVERROR(EAGAIN)) {
            continue;
        } else {
            break;
        }
        if (count-- <= 0) {
            break;
        }

    }
    av_packet_free(&pkt);
    file.close();
    avformat_close_input(&ctx);
}