#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/fb.h>
#include "androidlib.h"

static int uinput_fd = -1;
struct uinput_user_dev uinp;
struct input_event event;

int init_uinput_dev() {
    // Temporary variable

    if(uinput_fd != -1){
        LOGD("uinoute device was inited");
        return 1;
    }

    int i = 0;

    struct fb_var_screeninfo fb_var;
    int fd = open(DEV_GRAPHICS, O_RDONLY);

    if (fd < 0) {
        LOGE("open %s failed", DEV_GRAPHICS);
        return 2;
    }

    fd = ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);

    if (fd < 0) {
        LOGE("ioctl failed");
        return 2;
    }
    close(fd);

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
    uinp.absmin[ABS_X] = 0;
    uinp.absmax[ABS_X] = fb_var.xres;
    uinp.absmin[ABS_Y] = 0;
    uinp.absmax[ABS_Y] = fb_var.yres;
    uinp.absmin[ABS_MT_POSITION_X] = 0;
    uinp.absmax[ABS_MT_POSITION_X] = fb_var.xres;
    uinp.absmin[ABS_MT_POSITION_Y] = 0;
    uinp.absmax[ABS_MT_POSITION_Y] = fb_var.yres;
    uinp.absmax[ABS_MT_SLOT] = 9;

    // Setup the uinput device
    int ret = ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);
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


    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
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

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_VOLUMEDOWN);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_VOLUMEUP);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_HOME);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_HOMEPAGE);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_MENU);
    if(ret){
        ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    /* Create input device into input sub-system */
    write(uinput_fd, &uinp, sizeof(uinp));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        printf("Unable to create UINPUT device.");
        return -1;
    }
    return 1;
}

int write_event(__u16 type, __u16 code , __s32 value){

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = type;
    event.code = code;
    event.value = value;
    return write(uinput_fd,&event,sizeof(event));
}

int write_key_event(int keycode){
    return write_event(EV_KEY, keycode, 1) &&
           write_event(EV_SYN, SYN_REPORT, 0) &&
           write_event(EV_KEY, keycode, 0) &&
           write_event(EV_SYN, SYN_REPORT, 0);
}

int write_volume_up() {
    return write_key_event(KEY_VOLUMEUP);
}

int write_volume_down() {
    return write_key_event(KEY_VOLUMEDOWN);
}

int write_home_event(){
    return write_key_event(KEY_HOME);
}

int write_home_page_event(){
    return write_key_event(KEY_HOMEPAGE);
}

int write_menu_event(){
    return write_key_event(KEY_MENU);
}


int write_back_event() {
    return write_key_event(KEY_BACK);
}

int i = 0;
int write_click_event(int x, int y)
{
    struct input_event event;
    int ret;

    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_ABS;
    event.code = ABS_MT_TRACKING_ID;
    event.value = i;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_X;
    event.value = x;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_Y;
    event.value = y;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOUCH;
    event.value = 1;
    ret = write(uinput_fd, &event, sizeof(event));
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

    event.type = EV_ABS;
    event.code = ABS_MT_TRACKING_ID;
    event.value = -1;
    ret = write(uinput_fd,&event,sizeof(event));
    if(ret == -1){
        ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOUCH;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
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
    i++;
    return 0;
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



