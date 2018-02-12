#!/bin/bash

video=/dev/video1

eval $(ffprobe -v error -of flat=s=_ -select_streams v:0 -show_entries stream=height,width $video)

#W=${streams_stream_0_width}
#H=${streams_stream_0_height}

W=320
H=240
R=120

echo $video $W $H >&2


ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -f v4l2              \
    -framerate $R        \
    -video_size ${W}x${H}\
    -i $video            \
    -vf scale=$W:$H      \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
./rolling $W $H |        \
ffplay                   \
   -loglevel 0           \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${W}x${H} \
   -framerate 120        \
   -autoexit             \
   -i -
