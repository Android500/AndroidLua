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

    int i = 0;

    int w = 720;
    int h = 1280;

    struct fb_var_screeninfo fb_var;
    int fd = open(DEV_GRAPHICS, O_RDONLY);

    if (fd < 0) {
        LOGE("open %s failed", DEV_GRAPHICS);
    } else {
        fd = ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
        if (fd < 0) {
            LOGE("ioctl failed");
            return 2;
        } else {
            w = fb_var.xres;
            h = fb_var.yres;
        }
        close(fd);
    }
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
    uinp.absmax[ABS_X] = w;
    uinp.absmin[ABS_Y] = 0;
    uinp.absmax[ABS_Y] = h;
    uinp.absmin[ABS_MT_POSITION_X] = 0;
    uinp.absmax[ABS_MT_POSITION_X] = fb_var.xres;
    uinp.absmin[ABS_MT_POSITION_Y] = 0;
    uinp.absmax[ABS_MT_POSITION_Y] = fb_var.yres;
    uinp.absmax[ABS_MT_SLOT] = 9;

    // Setup the uinput device
    int ret = ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_X);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Y);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }


    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOOL_MOUSE);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOOL_PEN);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_BACK);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_VOLUMEDOWN);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_VOLUMEUP);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_HOME);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_HOMEPAGE);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ioctl(uinput_fd, UI_SET_KEYBIT, KEY_MENU);
    if (ret) {
        ALOGE("ioctl fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    /* Create input device into input sub-system */
    write(uinput_fd, &uinp, sizeof(uinp));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        printf("Unable to create UINPUT device.");
        return -1;
    }
    LOGE("%s fd: %d", __FUNCTION__, uinput_fd);

    return 1;
}

int write_event(__u16 type, __u16 code, __s32 value) {

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = type;
    event.code = code;
    event.value = value;
    int ret = write(uinput_fd, &event, sizeof(event));
    return ret;
}

int write_key_event(int keycode) {
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

int write_home_event() {
    return write_key_event(KEY_HOME);
}

int write_home_page_event() {
    return write_key_event(KEY_HOMEPAGE);
}

int write_menu_event() {
    return write_key_event(KEY_MENU);
}


int write_back_event() {
    return write_key_event(KEY_BACK);
}

int i = 0;

int write_click_event(int x, int y) {
    touchDown(x, y);
    touchUp(x, y);
    return 0;
}

int touchDown(int x, int y){
    struct input_event event;
    int ret;
    LOGE("fd: %d", uinput_fd);
    // Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = EV_ABS;
    event.code = ABS_MT_TRACKING_ID;
    event.value = i;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d) error: %d", __FUNCTION__, __LINE__, ret);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_X;
    event.value = x;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_Y;
    event.value = y;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOUCH;
    event.value = 1;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }
}
int touchScroll(int x, int y){
    struct input_event event;
    int ret;
    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_X;
    event.value = x;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_ABS;
    event.code = ABS_MT_POSITION_Y;
    event.value = y;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }
}

int touchUp(int x, int y){
    struct input_event event;
    int ret;
    event.type = EV_ABS;
    event.code = ABS_MT_TRACKING_ID;
    event.value = -1;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_KEY;
    event.code = BTN_TOUCH;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(uinput_fd, &event, sizeof(event));
    if (ret == -1) {
        ALOGE("write fail:%s(%d)", __FUNCTION__, __LINE__);
        return -1;
    }
}

int destroy() {
    if (uinput_fd < 0)
        return -1;
    if (ioctl(uinput_fd, UI_DEV_DESTROY) < 0)
        LOGE(0, "error: ioctl");

    close(uinput_fd);
    return 0;
}



