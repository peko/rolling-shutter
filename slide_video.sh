#!/bin/bash

video=${1:-videos/video_001.mp4}

eval $(ffprobe -v error -of flat=s=_ -select_streams v:0 -show_entries stream=height,width $video)

W=${streams_stream_0_width}
H=${streams_stream_0_height}

echo $video $W $H >&2


ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -i $video            \
    -vf crop=33:$H,transpose=1 \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
./sliding $W $H |        \
ffplay                   \
   -loglevel 0           \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -vf vflip             \
   -video_size ${W}x${H} \
   -framerate 120        \
   -autoexit             \
   -i -
