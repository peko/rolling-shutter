#!/bin/bash

S=/dev/video0

W=640
H=480
D=640

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
./strip $W $H $D |                       \
ffplay                                   \
   -loglevel 0                           \
   -hide_banner                          \
   -f rawvideo                           \
   -pixel_format rgb24                   \
   -video_size ${D}x${H}                 \
   -framerate 120                        \
   -i -

