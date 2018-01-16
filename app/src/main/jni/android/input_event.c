#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
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
    uinp.id.bustype = BUS_VIRTUAL;

    // 设置设备关注的Event
    if (ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) < 0)
        LOGE("error EV_KEY");


    if (ioctl(uinput_fd, UI_SET_EVBIT, EV_REL) < 0)
        LOGE("error EV_REL");
    if (ioctl(uinput_fd, UI_SET_RELBIT, REL_X) < 0)
        LOGE("error REL_X");
    if (ioctl(uinput_fd, UI_SET_RELBIT, REL_Y) < 0)


    ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_X);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_Y);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_MT_POSITION_X);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_MT_POSITION_Y);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_MT_SLOT);
    ioctl(uinput_fd, UI_SET_EVBIT, ABS_MT_TRACKING_ID);

    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE) < 0)
        LOGE("error BTN_MOUSE");
    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH) < 0)
        LOGE("error BTN_TOUCH");

    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        LOGE("error BTN_LEFT");
    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MIDDLE) < 0)
        LOGE("error BTN_MIDDLE");
    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
        LOGE("error BTN_RIGHT");
    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_FORWARD) < 0)
        LOGE("error BTN_FORWARD");
    if (ioctl(uinput_fd, UI_SET_KEYBIT, BTN_BACK) < 0)
        LOGE("error BTN_BACK");

    /* Create input device into input sub-system */
    write(uinput_fd, &uinp, sizeof(uinp));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        printf("Unable to create UINPUT device.");
        return -1;
    }
    return 1;
}


void send_a_button()
{
    // Report BUTTON CLICK - PRESS event
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = 188;
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
    event.code = 188;
    event.value = 0;

    write(uinput_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinput_fd, &event, sizeof(event));
}


void write_event(__u16 type, __u16 code, int value) {
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


//    struct input_event ev;
//    memset(&ev, 0, sizeof(struct input_event));
//
//    ev.type = EV_ABS;
//    ev.code = ABS_X;
//    ev.value = x;
//
//    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
//        printf("move error\n");
//    }
//
//    memset(&ev, 0, sizeof(struct input_event));
//    ev.type = EV_ABS;
//    ev.code = ABS_X;
//    ev.value = y;
//
//    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
//        printf("move error\n");
//    }
//
//    memset(&ev, 0, sizeof(struct input_event));
//    ev.type = EV_SYN;
//    ev.code = SYN_REPORT;
//    ev.value = 0;
//    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
//        printf("move error\n");
//
//    }


//
//    if (uinput_fd > 0) {
//        ioctl(uinput_fd, UI_DEV_DESTROY);
//        close(uinput_fd);
//        uinput_fd = -1;
//    }
}


void mouse_move(int dx, int dy) {
    struct input_event ev;
    memset(&ev, 0, sizeof(struct input_event));

    ev.type = EV_REL;
    ev.code = REL_X;
    ev.value = dx;

    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
        printf("move error\n");
    }

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_REL;
    ev.code = REL_Y;
    ev.value = dy;

    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
        printf("move error\n");
    }

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0) {
        printf("move error\n");

    }

}


void mouse_left_click()
{
    struct input_event ev;

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_MSC;
    ev.code = MSC_SCAN;
    ev.value = 0x90001;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf(" error\n");
    }
    memset(&ev, 0, sizeof(struct input_event));

    ev.type = EV_KEY;
    ev.code = 0x110;
    ev.value = 0x1;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf(" error\n");
    }

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf("click  error\n");
    }
    memset(&ev, 0, sizeof(struct input_event));

    ev.type = EV_MSC;
    ev.code = MSC_SCAN;
    ev.value = 0x90001;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf(" error\n");
    }

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_KEY;
    ev.code = 0x110;
    ev.value = 0x0;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf(" error\n");
    }

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(uinput_fd, &ev, sizeof(struct input_event)) < 0)
    {
        printf(" error\n");
    }
}
//
//int EVT_touch(int x, int y)
//{
//    int abs_x, abs_y;
//
//
//            calculateXY(x, y, &abs_x, &abs_y);
//    write_event2(ei.fd_touch, 3, 0, abs_x);
//    write_event2(ei.fd_touch, 3, 1, abs_y);
//    write_event2(ei.fd_touch, 1, 330, 1);
//    write_event2(ei.fd_touch, 0, 0, 0);
//
//}

int write_event2(int fd, int type, int code, int value)
{
    struct input_event event;

    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;
    if(write(fd, &event, sizeof(event)) < sizeof(event)) {
        LOGI("write event failed[%d]: %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

//void calculateXY(int x, int y, int *abs_x, int *abs_y)
//{
//    *abs_x = ei.abs_x_min +
//             (int)((x * (float)(ei.abs_x_max - ei.abs_x_min)) / 720 + 0.5);
//    *abs_y = ei.abs_y_min +
//             (int)((y * (float)(ei.abs_y_max - ei.abs_y_min)) / 1280 + 0.5);
//}



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

