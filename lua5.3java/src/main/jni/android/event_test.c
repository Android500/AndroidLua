#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <android/log.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/stat.h>
#include "linux/uinput.h"
#include "input_event.h"
#include "androidlib.h"


/* global variable */
static int uinput_fd = -1;
struct uinput_user_dev uinput; // uInput device structure

/* Setup the uinput device */
int setup_uinput_device()
{
    //int i=0;
    int ret;

    // Open the input device
    uinput_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
    if (uinput_fd < 0){
        ALOGE("Unable to open /dev/uinput/n");
        return -1;
    }

    memset(&uinput,0,sizeof(uinput)); // Intialize the uInput device to NULL
    strncpy(uinput.name, "vtouch", UINPUT_MAX_NAME_SIZE);
    uinput.id.version = 4;
    uinput.id.bustype = BUS_USB;


    uinput.absmin[ABS_X] = 0;
    uinput.absmax[ABS_X] = 1280;
    uinput.absmin[ABS_Y] = 0;
    uinput.absmax[ABS_Y] = 720;

    // Setup the uinput device
    ret = ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_X);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Y);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOOL_MOUSE);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOOL_PEN);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_BACK);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    /*
     ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
     ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);

     ioctl(uinput_fd, UI_SET_RELBIT, REL_X);
     ioctl(uinput_fd, UI_SET_RELBIT, REL_Y);

     for (i=0; i < 256; i++) {
        ioctl(uinput_fd, UI_SET_KEYBIT, i);
     }

     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MIDDLE);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_FORWARD);
     ioctl(uinput_fd, UI_SET_KEYBIT, BTN_BACK);
     */


    // Create input device into input sub-system
    if(write(uinput_fd, &uinput, sizeof(uinput))== -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    if(ioctl(uinput_fd, UI_DEV_CREATE))
    {
        ALOGE("Unable to create UINPUT device.");
        return -1;
    }

    return 0;
}

/* Destroy the uinput device */
int destroy_uinput_device()
{
    // Destroy the uinput device
    ioctl(uinput_fd, UI_DEV_DESTROY);

    // Close the uinput device
    close(uinput_fd);
}

int write_touch_move(int x, int y)
{
    struct input_event event;
    int ret;

    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = x;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_Y;
    event.value = y;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOOL_PEN;
    event.value = 1;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}


int write_touch_hide(void)
{
    struct input_event event;
    int ret;

    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = 0;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_Y;
    event.value = 0;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOOL_PEN;
    event.value = 0;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }


    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}



int write_key(int key, int down)
{
    struct input_event event;
    int ret;

    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_KEY;
    event.code = key;
    event.value = down;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }


    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}

int write_touch_button(int x, int y, int down)
{
    struct input_event event;
    int ret;

    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_ABS;
    event.code = ABS_X;
    event.value = x;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_Y;
    event.value = y;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOUCH;
    event.value = down;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}



#define DIS_PIXEL 1
#define DUR_TIME  5

int main() {
    int i;
    // Initialize uinput device
    if(setup_uinput_device()){
        ALOGE("setup uinput device fail");
        return -1;
    }

    // Send gsensor event
    int write_count=0;
    int count =0;
    int x=0,y=0;

    count = 0;
    while(1){  // for touch screen

        x = 10;
        y = 10;

        while(x <1270){
            write_touch_move(x,y);
            x+=DIS_PIXEL;
            usleep(DUR_TIME*1000);
        }

        y+=DIS_PIXEL;
        while(y < 710){
            write_touch_move(x,y);
            y+=DIS_PIXEL;
            usleep(DUR_TIME*1000);
        }

        x -=DIS_PIXEL;
        while(x > 10){
            write_touch_move(x,y);
            x-=DIS_PIXEL;
            usleep(DUR_TIME*1000);
        }

        /*
        y -=DIS_PIXEL;
        while(y > 10){
            write_touch_move(x,y);
            y-=DIS_PIXEL;
            usleep(DUR_TIME*1000);
        }
        */

        write_touch_hide();

        sleep(5);

        /*
        count++;
        if(count>5){
            while(1){
                sleep(60);
            }

        }
        */
    }

    // Destroy uinput device
    destroy_uinput_device();
    return 0;
}