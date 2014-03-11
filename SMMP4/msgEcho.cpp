#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spcaframe.h"

static struct frame_t headerframe;
static struct client_t* callback;

int get_headframelen(void)
{
    return sizeof(struct frame_t);
}

void set_headframedata(char *p,int len)
{
    memcpy(&headerframe,p,len);
//    printf(" key %s nb %d width %d height %d times %dms size %d \n",
//                                    headerframe.header, headerframe.nbframe, headerframe.w,
//                                    headerframe.h, headerframe.deltatimes, headerframe.size);
}

int get_dataframelen(void)
{
    if (headerframe.size && !headerframe.wakeup)
        return headerframe.size;
    else
        return 0;
}


int get_callbackmessagelen(void)
{
    return sizeof(struct client_t);
}

void init_callbackmessage(void) {
       callback = (struct client_t*) malloc(sizeof(struct client_t));

        char key[4] = { 'O', 'K', '\0', '\0' };
        int x = 128;
        int y = 128;
        unsigned char sleepon = 0;
        unsigned char bright = 0;
        unsigned char contrast = 0;
        unsigned char exposure = 0;
        unsigned char colors = 0;
        unsigned char size = 0;
        unsigned char fps = 0;
        memcpy(callback->message, key, 4);
        callback->x = x;
        callback->y = y;
        callback->updobright = bright;
        callback->updocontrast = contrast;
        callback->updoexposure = exposure;
        callback->updocolors = colors;
        callback->sleepon = sleepon;
        callback->updosize = size;
        callback->fps = fps;
}

void get_callbackmessage(char *p, int len)
{
    memcpy(p,callback,len);
}

void reset_callbackmessage(void) {
    if (headerframe.acknowledge)
    {
        callback->updobright = 0;
        callback->updocontrast = 0;
        callback->updoexposure = 0;
        callback->updocolors = 0;
        callback->sleepon = 0;
        callback->updosize = 0;
        callback->fps = 0;
    }
}
