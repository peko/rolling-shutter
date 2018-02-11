#!/bin/bash

video=${1:-videos/video_01.mp4}

eval $(ffprobe -v error -count_frames -of flat=s=_ -select_streams v:0 -show_entries stream=height,width,nb_read_frames $video)

W=${streams_stream_0_width}
H=${streams_stream_0_height}
D=${streams_stream_0_nb_read_frames}

echo $video $W $H $D>&2

W=360
H=480

ffmpeg                   \
    -loglevel 0          \
    -hide_banner         \
    -i $video            \
    -vf scale=$W:$H      \
    -pix_fmt rgb24       \
    -f rawvideo          \
    - |                  \
./strip $W $H $D |       \
ffmpeg                   \
   -loglevel 0           \
   -hide_banner          \
   -f rawvideo           \
   -pixel_format rgb24   \
   -video_size ${D}x${H} \
   -framerate 60         \
   -i -                  \
   -c:v libx264          \
   -preset slow -crf 22  \
   ./results/$(basename "$video")

