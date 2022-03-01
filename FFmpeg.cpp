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

static int unwrap_packet(AVPacket *source, AVFrame *dest);

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
    AVFormatContext *input_format_ctx = nullptr;
    auto res = avformat_open_input(&input_format_ctx, DEVICE, fmt, &options);
    if (res < 0) {
        av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
        return;
    }
    AVFormatContext *output_format_ctx = nullptr;
    avformat_alloc_output_context2(&output_format_ctx, nullptr, "h264", "cam.h264");
    avformat_init_output(output_format_ctx, nullptr);
    auto output_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    auto output_codec_ctx = avcodec_alloc_context3(output_codec);
    auto output_stream = avformat_new_stream(output_format_ctx, output_codec);
    avcodec_parameters_copy(output_stream->codecpar,input_format_ctx->streams[0]->codecpar);
    avformat_write_header(output_format_ctx, nullptr);
    if (res < 0) {
        av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
        return;
    }
    auto params = input_format_ctx->streams[0]->codecpar;
    auto bufferSize = av_image_get_buffer_size((AVPixelFormat) (params->format), params->width, params->height, 1);
    auto pkt = av_packet_alloc();
    auto frame = av_frame_alloc();
    auto count = 60;
    while (true) {
        auto ret = av_read_frame(input_format_ctx, pkt);
        if (ret == 0) {
            unwrap_packet(pkt, frame);
            res = avcodec_send_frame(output_codec_ctx, frame);
            if (res == AVERROR(EAGAIN)) {
                av_log(nullptr, AV_LOG_INFO, "send frame continue");
                continue;
            }
            if (res < 0) {
                av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
                return;
            }
            while (true) {
                res = avcodec_receive_packet(output_codec_ctx, pkt);
                if (res == AVERROR(EAGAIN)) {
                    break;
                }
                if (res < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
                    return;
                }
                pkt->stream_index = 0;
                res = av_interleaved_write_frame(output_format_ctx, pkt);
                if (res < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
                    return;
                }
            }
            if (res < 0) {
                av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(res));
                return;
            }
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
    avformat_close_input(&input_format_ctx);
    av_write_trailer(output_format_ctx);
}


static int unwrap_packet(AVPacket *source, AVFrame *dest) {
    static bool opened = false;
    static auto passthroughDecoder = avcodec_find_decoder(AV_CODEC_ID_WRAPPED_AVFRAME);
    static auto passthroughDecoderCtx = avcodec_alloc_context3(passthroughDecoder);
    if (!opened) {
        opened = true;
        auto result = avcodec_open2(passthroughDecoderCtx, passthroughDecoder, nullptr);
        if (result < 0) {
            av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(result));
            return result;
        }
    }
    auto result = avcodec_receive_packet(passthroughDecoderCtx, source);
    if (result < 0) {
        av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(result));
        return result;
    }
    avcodec_receive_frame(passthroughDecoderCtx, dest);
    if (result < 0) {
        av_log(nullptr, AV_LOG_ERROR, "%s", av_err2str(result));
        return result;
    }
}