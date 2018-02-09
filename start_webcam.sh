#!/bin/bash

S=/dev/video0
W=640
H=480

ffmpeg                                   \
    -loglevel 0                          \
    -hide_banner                         \
    -f v4l2                              \
    -framerate 30                        \
    -video_size ${W}x${H}                \
    -i $S                                \
    -pix_fmt rgb24                       \
    -f rawvideo                          \
    - |                                  \
./strip |                                \
ffplay                                   \
   -loglevel 0                           \
   -hide_banner                          \
   -f rawvideo                           \
   -pixel_format rgb24                   \
   -video_size ${W}x${H}                 \
   -framerate 30                         \
   -i -

