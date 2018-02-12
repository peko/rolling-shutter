#!/bin/bash

video=${1:-videos/video_001.mp4}

eval $(ffprobe -v error -of flat=s=_ -select_streams v:0 -show_entries stream=height,width $video)

W=${streams_stream_0_width}
H=${streams_stream_0_height}

#W=800
#H=600

echo $video $W $H >&2


ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -i $video            \
    -vf scale=$W:$H      \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
./rolling $W $H |        \
ffmpeg                   \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${W}x${H} \
   -framerate 30         \
   -i -                  \
   -c:v libx264          \
   -preset slow -crf 22  \
   -y                    \
   ./results/$(basename "$video")
