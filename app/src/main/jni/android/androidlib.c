//
// Created by huangxin on 2018/1/13.
//

#include "unistd.h"
#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <android/log.h>
#include <dirent.h>
#include <linux/input.h>
#include <malloc.h>
#include "androidlib.h"
#include "input_event.h"
#include "shell_cmd.h"



/*simple sleep*/
static int system_sleep(lua_State *L) {
    long secs = lua_tointeger(L, -1); /*获取参数/毫秒*/
    usleep(secs * 1000 * 1000);        ///毫秒转微秒
    return 0;                         /*返回0个值，所以为0*/
}





//sendevent /dev/input/event5 3 57 2 && sendevent /dev/input/event5 3 53 266 && sendevent /dev/input/event5 3 54 939 && sendevent /dev/input/event5 1 330 1 && sendevent /dev/input/event5 0 0 0 && sendevent /dev/input/event5 3 57 0 && sendevent /dev/input/event5 1 330 0 && sendevent /dev/input/event5 0 0 0
//sendevent /dev/input/event5 1 330 1 && sendevent /dev/input/event5 3 48 81 && sendevent /dev/input/event5 3 50 4 && sendevent /dev/input/event5 3 53 438 && sendevent /dev/input/event5 3 54 880 && sendevent /dev/input/event5 0 2 0 && sendevent /dev/input/event5 0 0 0 && sendevent /dev/input/event5 1 330 0 && sendevent /dev/input/event5 0 2 0 && sendevent /dev/input/event5 0 0 0


/**
 *
 * 模拟点击事件
 *
 * */
static int system_click(lua_State *L) {

    int x = lua_tointeger(L, -2); /*获取参数X*/
    int y = lua_tointeger(L, -1); /*获取参数Y*/
    LOGD("input touch Event: x=%d,y=%d", x, y);

    if (init_uinput_dev() < 0) {
        LOGD("init uinput dev failed");
        return 0;
    }

    usleep(100000);//0.1s
    int result = write_click_event(x, y);
    if(result < 0)
        LOGE("error emulate click event");

    return 0;
}

/*simple sleep*/
static int system_getScreenSize(lua_State *L) {

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

    lua_pushnumber(L, fb_var.xres);
    lua_pushnumber(L, fb_var.yres);

    close(fd);
    return 2;
}

static int system_back(lua_State *L) {

    if (init_uinput_dev() < 0) {
        LOGD("init uinput dev failed");
        return 0;
    }

    int result = write_back_event();
    if(result < 0)
        LOGE("error emulate back event");
    return 0;
}

static int system_volumeUp(lua_State *L){
    int result = write_volume_up();
    if(result < 0)
        LOGE("error emulate volume_up event");
    return 0;
}

static int system_volumeDown(lua_State *L){
    int result = write_volume_down();
    if(result < 0)
        LOGE("error emulate volume_down event");
    return 0;
}

static int system_inputText(lua_State *L){
    const char *text = lua_tostring(L, -1); /*获取参数X*/
    int result = input_text(text);
    if(result < 0)
        LOGE("error emulate inputText event");
    return 0;
}

static int system_home(lua_State *L){

    if (init_uinput_dev() < 0) {
        LOGD("init uinput dev failed");
        return 0;
    }

    int result = write_home_event();
    if(result < 0)
        LOGE("error emulate home event");
    return 0;
}

static int system_homePage(lua_State *L){

    if (init_uinput_dev() < 0) {
        LOGD("init uinput dev failed");
        return 0;
    }

    int result = write_home_page_event();
    if(result < 0)
        LOGE("error emulate home page event");
    return 0;
}

static int system_menu(lua_State *L){
    int result = write_menu_event();
    if(result < 0)
        LOGE("error emulate menu event");
    return 0;
}

static const struct luaL_Reg libs[] = {
        {"sleep",         system_sleep},
        {"getScreenSize", system_getScreenSize},
        {"click",         system_click},
        {"back",         system_back}, //测试未通过
        {"volumeUp",         system_volumeUp},
        {"volumeDown",         system_volumeDown},
        {"inputText",         system_inputText},
        {"home",         system_home},
        {"homePage",         system_homePage},
        {"menu",         system_menu},
        {NULL, NULL}  /*the end*/
};

int luaopen_system(lua_State *L) {
    /*注册lib， 上面luaopen_名称 跟下面注册的名称要一致, 还要和编译的.so文件名一致*/
    luaL_register(L, "system", libs);
    return 1;
}
//cat /dev/graphics/fb0 > /storage/emulated/0/DCIM/screen.raw
//ls -l /dev/graphics/fb0