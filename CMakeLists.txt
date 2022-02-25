cmake_minimum_required(VERSION 3.21)
project(FFDemo)

set(CMAKE_CXX_STANDARD 11)

include_directories(E:/Work/x264/include)
include_directories(E:/Work/ffmpeg/include)

find_library(X264 libx264 PATHS E:/Work/x264 REQUIRED)

# 需要注意下面这条，我以为 find_library 可以同时寻找同一个路径下的多个不同的库，比如 avformat avcodec 这是错误的观点。多个库必须多次调用 find_library
#find_library(FFMPEG NAMES avcodec [avformat avutil]  [NAMES_PER_DIR] PATHS E:/Work/ffmpeg/bin REQUIRED)
find_library(CODEC avcodec PATHS E:/Work/ffmpeg/bin REQUIRED NO_CACHE)

find_library(FORMAT avformat PATHS E:/Work/ffmpeg REQUIRED NO_CACHE) # 注意，不需要填写最精确的路径，比如这里省略了 bin 路径完全可以找到
find_library(UTIL avutil PATHS E:/Work/ffmpeg REQUIRED NO_CACHE)
find_library(DEVICE avdevice PATHS E:/Work/ffmpeg REQUIRED NO_CACHE)

message(WARNING "this is a debug message for test print info to terminal ${CODEC}  ${FORMAT} ${UTIL} ${X264} ${DEVICE}")

add_executable(FFDemo main.cpp FFmpeg.cpp FFmpeg.h)

target_link_libraries(FFDemo ${CODEC}  ${FORMAT} ${UTIL} ${X264} ${DEVICE})