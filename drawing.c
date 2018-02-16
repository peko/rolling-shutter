#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>

FILE* fo;
pthread_t tid;
cairo_surface_t* surface;
cairo_t* cr;
unsigned char* data; 

int W = 640;
int H = 360;

void init(){
    fo = fdopen(dup(fileno(stdout)), "wb"); 

    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 640, 360);
    cr = cairo_create(surface);
    data = cairo_image_surface_get_data(surface);
};

void cleanup(){
    fclose(fo);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
};

void draw() {
    static int n=0;
    static int x=0;
    static int dx=1;
    static int y=0;
    static int dy=1;
    
    cairo_select_font_face(
        cr, "serif",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_source_rgb (cr, 0.1, 0.1, 0.1);
    cairo_paint(cr);
    
    cairo_set_font_size(cr, 18.0);
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_move_to(cr, x, y);
    char buf[128];
    sprintf(buf, "%d", n++);
    cairo_show_text(cr, buf);

    for(int i=0; i<5; i++) {   
        cairo_move_to(cr, rand()%W, rand()%H);
        cairo_line_to(cr, rand()%W, rand()%H);
        cairo_stroke(cr);
    }

    x+=dx; y+=dy;
    if(x>W || x<0) dx*=-1;
    if(y>H || y<0) dy*=-1;
   
};

const struct timespec delay = {0, 30*1000000L};
int done = 0;
void* thread(void* arg) {
    while(!done) {
        draw();
        fwrite(data, 1, W*H*4, fo); 
        nanosleep(&delay, NULL);
    }
}

void sigint(int dummy) {
    fprintf(stderr, "CTRL-C");
    done = 1;
}

int main() {

    signal(SIGINT, sigint); 

    init();

    pthread_create(&tid, NULL, &thread, NULL);
    pthread_exit(0);

    cleanup();

    return 0;
}
