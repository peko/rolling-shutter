#!/bin/bash

video=${1:-videos/video_001.mp4}

rm results/*

echo calculate duration of "$video"
eval $(ffprobe -v error -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames "$video")
#eval $(ffprobe -v error -count_frames -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames $video)

name=$(basename "$video")
ext="${name##*.}"
name="${name%.*}"

W=${streams_stream_0_width}
H=${streams_stream_0_height}
#D=${streams_stream_0_nb_read_frames}
D=320

echo "$video" ">>" $name.png $W $H $D >&2
PERIOD="200.0"
RADIUS="iw/2.0"
X="iw/2.0+sin(n/$PERIOD)*$RADIUS"
# X="iw/2.0"
HH=$((H/2))
CROP="crop=2:$H:$X:0,scale=1:$HH,transpose=1"
ffmpeg                   \
    -hide_banner         \
    -loglevel 0          \
    -i "$video"          \
    -vf deshake          \
    -vf "$CROP"          \
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
   ./results/strip_%03d.png

montage               \
    ./results/*.png   \
    -mode Concatenate \
    results/montage.png
feh --auto-zoom ./results/montage.png

# ffplay \
#     -hide_banner \
#     -loglevel 0  \
#     ./results/$name.png
