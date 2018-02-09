#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  640
#define HEIGHT 480
#define LENGTH 640
#define COLORS   3
#define LINE_SIZE  WIDTH*COLORS
#define FRAME_SIZE LINE_SIZE*HEIGHT
#define STRIP_WIDTH 15

FILE* fi;
FILE* fo;

uint32_t buffer_length = LENGTH;
uint8_t  buffer[FRAME_SIZE];
uint8_t  out_buffer[FRAME_SIZE];
uint8_t* current_buffer;

uint32_t current_pos = STRIP_WIDTH;

int main(int argc, char* argv[]) {

    fi = fdopen(dup(fileno(stdin )), "rb");
    fo = fdopen(dup(fileno(stdout)), "wb"); 

    while(1) {
        current_buffer = buffer;

        if(!fread(current_buffer, 1, FRAME_SIZE, fi)) break;
        for(uint32_t y=0; y<HEIGHT; y++) {
            for(int32_t x=-STRIP_WIDTH; x<=STRIP_WIDTH; x++) {
                uint32_t f = y*LINE_SIZE + LINE_SIZE/2 + (x + ceil(sin(current_pos/100.0)*300))*COLORS;
                uint32_t t = y*LINE_SIZE + (current_pos+x)*COLORS;
                
                // out_buffer[t+0] = (out_buffer[t+0]+current_buffer[f+0])/2;
                // out_buffer[t+1] = (out_buffer[t+1]+current_buffer[f+1])/2;
                // out_buffer[t+2] = (out_buffer[t+2]+current_buffer[f+2])/2;
                
                out_buffer[t+0] = current_buffer[f+0];
                out_buffer[t+1] = current_buffer[f+1];
                out_buffer[t+2] = current_buffer[f+2];
            }
        }
        fwrite(out_buffer, 1, FRAME_SIZE, fo);

        current_pos = current_pos+1;
        if (current_pos > WIDTH) current_pos = STRIP_WIDTH;
        
    }
    
    fclose(fi);
    fclose(fo);

    return 0;
}

