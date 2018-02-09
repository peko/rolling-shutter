#!/bin/bash

S=${1:-videos/video_01.mp4}

W=640
H=480

ffmpeg                                   \
    -loglevel 0                          \
    -hide_banner                         \
    -i $S                                \
    -vf scale=${W}:${H}                  \
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

