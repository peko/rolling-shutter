#!/bin/bash

video=${1:-videos/video_001.mp4}

echo calculate duration of $video
# eval $(ffprobe -v error -count_frames -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames $video)
eval $(ffprobe -v error -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames $video)
name=$(basename "$video")
ext="${name##*.}"
name="${name%.*}"

W=${streams_stream_0_width}
H=${streams_stream_0_height}
D=${streams_stream_0_nb_read_frames}

SCALE=2
W2=$((W/2))
HH=$((H/SCALE))

echo "$video" ">>" $name.png $W $H $D >&2
D=2048
PERIOD="100.0/$SCALE"
RADIUS="iw/3.0"
POS="iw/2.0+sin(n/$PERIOD)*$RADIUS"

ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -i "$video"          \
    -vf "crop=2:$H:$POS:0:,scale=1:$HH,transpose=1" \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
pv |                     \
ffmpeg                   \
   -loglevel 0           \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${HH}x${D}\
   -i -                  \
   -y                    \
   -vf transpose=2       \
   ./results/${name}_%03d.png

echo ./results/$name.png
# ffplay \
#     -hide_banner \
#     -loglevel 0  \
#     ./results/$name.png
