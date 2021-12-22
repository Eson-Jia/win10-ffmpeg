#include <iostream>

extern "C"{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/log.h"
#include "x264.h"
}

int main() {
    x264_param_t param;
    x264_param_default(&param);
    av_log_set_level(AV_LOG_TRACE);
    auto codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (codec){
        std::cout << "success!!!" << codec->name << std::endl;
    }else{
        std::cout << "failed!!!" << std::endl;
    }
    return 0;
}