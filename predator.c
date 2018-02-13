#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_WIDTH  4096
#define MAX_HEIGHT 4096
#define COLORS        3

#define err(txt) fprintf(stderr,(txt"\n"))

int32_t  width    =  0;
int32_t  height   =  0;
int64_t duration  =  0; 

FILE* fi;
FILE* fo;

uint8_t* buffer = NULL;
uint8_t* frame  = NULL;

uint32_t line_size;
uint32_t frame_size;
uint64_t buffer_size;

void usage() {
    err("Usage:");
    err("<raw rgb frames> | strip <width> <height>| <raw rgb frames>");
    err("ffpmeg ... | strip ... | ffplay ...");
    err("Detailed examples can be found at bash scripts");
}

int init(int argc, char** argv) {

    if(argc != 3) {
        usage();
        return 0;
    }
    
    width    = atoi(argv[1]);
    height   = atoi(argv[2]);
    duration = height;

    fprintf(stderr, "Stream: %dx%dpx\n", width, height);
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
    buffer_size = duration   * frame_size;

    fprintf(stderr, "line size  : %d\n", line_size  ); 
    fprintf(stderr, "frame size : %d\n", frame_size ); 
    fprintf(stderr, "buffer size: %d\n", buffer_size); 
    fprintf(stderr, "duration   : %d\n", duration   ); 
    return 1;
}

void run() {

    uint32_t bytes;

    uint8_t* buffer = calloc(buffer_size, 1);
    uint8_t* out    = calloc(frame_size , 1);
    
    int32_t d = 0;
    while(fread(&buffer[d*frame_size], 1, frame_size, fi)) {        
        for(int32_t y=0; y<height; y++) {
            char type = 2; 
            for(int32_t x=0; x<width; x++) {
                
                int64_t trg = x*COLORS + y*line_size;
                uint8_t r = buffer[d*frame_size+trg+0];
                uint8_t g = buffer[d*frame_size+trg+1];
                uint8_t b = buffer[d*frame_size+trg+2];

                int32_t dr= d-(255-r)/48.0;
                int32_t dg= d-(255-g)/64.0;
                int32_t db= d-(255-b)/96.0;

                if(dr<0) dr+=duration;
                if(dg<0) dg+=duration;
                if(db<0) db+=duration;

                int64_t sr = x*COLORS + y*line_size + dr*frame_size;
                int64_t sg = x*COLORS + y*line_size + dg*frame_size;
                int64_t sb = x*COLORS + y*line_size + db*frame_size;
                
                sr %= buffer_size;
                sg %= buffer_size;
                sb %= buffer_size;
                
                trg %= frame_size;

                out[trg+0] = buffer[sr+0];
                out[trg+1] = buffer[sg+1];
                out[trg+2] = buffer[sb+2];

                // out[trg+0] = buffer[d*frame_size+trg+0]*buffer[sr+0]/255;
                // out[trg+1] = buffer[d*frame_size+trg+1]*buffer[sg+1]/255;
                // out[trg+2] = buffer[d*frame_size+trg+2]*buffer[sb+2]/255;
            }
        }
        d++;
        d%=duration;
        fwrite(out, 1, frame_size, fo);
   }
   free(buffer);
   free(out   );
}

void cleanup() {
    fprintf(stderr, "cleanup\n");
    
    if(fi!=NULL) fclose(fi);
    if(fo!=NULL) fclose(fo);
}

int main(int argc, char** argv) {

    if(init(argc, argv)) run();
    cleanup();

    return 0;
}

