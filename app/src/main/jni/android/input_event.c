#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <stdio.h>
#include "androidlib.h"

static int uinput_fd = -1;
struct uinput_user_dev uinp;
struct input_event event;
int init_uinput_dev() {
    // Temporary variable
    int i = 0;

    // Open the input device
    uinput_fd = open(DEV_UINPUT, O_WRONLY | O_NDELAY);
    if (uinput_fd < 0) {
        LOGE("Unable to open /dev/uinput");
        return -1;
    }

    memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
    strncpy(uinp.name, "AutomaticKey Dev", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 1;
    uinp.id.bustype = BUS_USB;

    // 设置设备关注的Event
    if(ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) < 0 )
        LOGE("error EV_KEY");
    if(ioctl(uinput_fd, UI_SET_EVBIT, EV_REL) < 0)
        LOGE("error EV_REL");
    if(ioctl(uinput_fd, UI_SET_RELBIT, REL_X) < 0 )
        LOGE("error REL_X");
    if(ioctl(uinput_fd, UI_SET_RELBIT, REL_Y) < 0 )
        LOGE("error REL_Y");

    for (i = 0; i < 256; i++) {
        if(ioctl(uinput_fd, UI_SET_KEYBIT, i) < 0)
            LOGE("error %d", i);
    }
    if(ioctl(uinput_fd, UI_SET_RELBIT, REL_Y) < 0)
        LOGE("error REL_Y");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE) < 0)
        LOGE("error BTN_MOUSE");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH) < 0)
        LOGE("error BTN_TOUCH");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE) < 0)
        LOGE("error BTN_MOUSE");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        LOGE("error BTN_LEFT");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MIDDLE) < 0)
        LOGE("error BTN_MIDDLE");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
        LOGE("error BTN_RIGHT");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_FORWARD) < 0)
        LOGE("error BTN_FORWARD");
    if(ioctl(uinput_fd, UI_SET_KEYBIT, BTN_BACK) < 0)
        LOGE("error BTN_BACK");

    /* Create input device into input sub-system */
    write(uinput_fd, &uinp, sizeof(uinp));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        printf("Unable to create UINPUT device.");
        return -1;
    }
    return 1;
}


void write_event(__u16 type, __u16 code, int value){
    struct input_event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;
    write(uinput_fd, &event, sizeof(event));
}

/**
 *
 * 模拟输入一个点击时间
 *
 * **/
void write_click_events(int x, int y) {
    // Move pointer to (100,100) location


    write_event( EV_ABS, REL_X, x); //set x coord
    write_event( EV_ABS, REL_Y, y); //set y coord
    write_event( EV_ABS, 24,100);
    write_event( EV_ABS, 28,1);
    write_event( 1, 330, 1); // touch down
    write_event( EV_ABS, 53,x);
    write_event( EV_ABS, 54,y);
    write_event( EV_ABS, 48,100);
    write_event( EV_ABS, 50,0);
    write_event( 0, 2,0);
    write_event( 0, 2,0);
    write_event( 0, 0,0);
    write_event( EV_ABS, 24,0);
    write_event( EV_ABS, 28,0);
    write_event( 1, 330,0); //touch up
    write_event( EV_ABS, 53,0);
    write_event( EV_ABS, 54,0);
    write_event( EV_ABS, 48,0);
    write_event( EV_ABS, 50,0);
    write_event( 0, 2,0);
    write_event( 0, 2,0);
    write_event( 0, 0,0);

    if(uinput_fd > 0) {
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
        uinput_fd = -1;
    }
}

void send_keyboard_event(int keycode) {
    // Report BUTTON CLICK - PRESS event
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = keycode;
    event.value = 1;
    write(uinput_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinput_fd, &event, sizeof(event));

    // Report BUTTON CLICK - RELEASE event
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = keycode;
    event.value = 0;

    write(uinput_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinput_fd, &event, sizeof(event));
}

