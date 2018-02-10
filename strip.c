#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_WIDTH  4096
#define MAX_HEIGHT 4096
#define COLORS        3

#define err(txt) fprintf(stderr,(txt"\n"))

int32_t  width    = 0;
int32_t  height   = 0;
int32_t  duration = 0;

FILE* fi;
FILE* fo;

uint8_t* buffer = NULL;
uint8_t* frame  = NULL;

uint32_t line_size;
uint32_t frame_size;
uint32_t buffer_size;

void usage() {
    err("Usage:");
    err("<raw rgb frames> | strip <width> <height> <duration>| <raw rgb frames>");
    err("ffpmeg ... | strip ... | ffplay ...");
    err("Detailed examples can be found at bash scripts");
}

int init(int argc, char** argv) {

    if(argc != 4) {
        usage();
        return 0;
    }
    
    width    = atoi(argv[1]);
    height   = atoi(argv[2]);
    duration = atoi(argv[3]);
    
    fprintf(stderr, "Stream: %dx%dpx %d frames\n", width, height, duration);
    if (width <=0 || width >MAX_WIDTH || 
        height<=0 || height>MAX_HEIGHT) {       
        err("Wrong width or height");
        return 0;
    }

    fi = fdopen(dup(fileno(stdin )), "rb");
    fo = fdopen(dup(fileno(stdout)), "wb"); 

    if(fi == NULL || fo == NULL) {
        err("Can not open sdtin or stdout");
        return 0;
    }

    line_size   = width      * COLORS;
    frame_size  = line_size  * height;
    buffer_size = frame_size * duration;
    
    buffer = calloc(buffer_size, 1);
    
    return 1;
}

void run() {
    uint32_t bytes;
    
    bytes = fread(buffer, 1, buffer_size, fi);
    fprintf(stderr, "%dG\n", bytes>>30);
    if(bytes == 0) return;

    uint32_t w2 = width>>1;
    uint8_t* out = calloc(duration * height * COLORS, 1);
    for(uint32_t t=0; t<1000; t++) {
        for(uint32_t y=0; y<height; y++) {
            for(uint32_t x=0; x<duration; x++) {
                uint32_t off = w2+sin(t/250.0)*w2;
                uint32_t src = off*COLORS + x*frame_size + y*line_size;
                uint32_t trg = x*COLORS                + y*duration*COLORS;
                out[trg+0] = buffer[src+0];
                out[trg+1] = buffer[src+1];
                out[trg+2] = buffer[src+2];
            }
        }
        bytes = fwrite(out, 1, duration*height*COLORS, fo);
   }
   free(out);
}

void cleanup() {
    fprintf(stderr, "cleanup\n");

    if(buffer!=NULL) free(buffer);
    
    if(fi!=NULL) fclose(fi);
    if(fo!=NULL) fclose(fo);
}

int main(int argc, char** argv) {

    if(init(argc, argv)) run();
    cleanup();

    return 0;
}

