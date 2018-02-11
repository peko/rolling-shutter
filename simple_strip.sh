#!/bin/bash

video=${1:-videos/video_001.mp4}

eval $(ffprobe -v error -count_frames -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames $video)

W=${streams_stream_0_width}
H=${streams_stream_0_height}
D=${streams_stream_0_nb_read_frames}
W2=$((W/2))

echo $video $W $H $D>&2


ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -i $video            \
    -vf crop=2:$H,scale=1:$H,transpose=1 \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
pv |                     \
ffplay                   \
   -loglevel 0           \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${H}x${D} \
   -vf transpose=2       \
   -framerate 30         \
   -autoexit             \
   -i -

