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

echo "$video" ">>" $name.png $W $H>&2
S=4
X="mod(n*$S\,iw)"
CROP="crop=$S:$H:$X:0,transpose=1"

ffmpeg                   \
    -hide_banner         \
    -loglevel 0          \
    -i "$video"          \
    -vf "$CROP"          \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
pv |                     \
ffmpeg                   \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${H}x${W} \
   -i -                  \
   -y                    \
   -vf transpose=2       \
   -framerate 1          \
   -c:v libx264          \
   -preset slow -crf 22  \
   ./results/$(basename "$video")

# ffplay \
#     -hide_banner \
#     -loglevel 0  \
#     ./results/$name.png
