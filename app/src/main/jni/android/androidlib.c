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


/*simple sleep*/
static int system_sleep (lua_State *L){
    long secs = lua_tointeger(L, -1); /*获取参数/毫秒*/
    usleep(secs * 1000);///毫秒转微秒
    return 0;                         /*返回0个值，所以为0*/
}

/**
 * 模拟点击事件
 * */
static int system_touch(lua_State *L){

    int x = lua_tointeger(L, -2); /*获取参数X*/
    int y = lua_tointeger(L, -1); /*获取参数Y*/
    LOGD("input touch Event: x=%d,y=%d", x, y);

    if(init_uinput_dev() < 0){
        LOGD("init uinput dev failed");
        return 0;
    }else{
        LOGE("success init uinput dev-");
    }
    sleep(2);
    write_click_events(x, y);
    return 0;
}

/*simple sleep*/
static int system_getScreenSize(lua_State *L){

    struct fb_var_screeninfo fb_var;
    int fd = open(DEV_GRAPHICS, O_RDONLY);

    if(fd < 0){
        LOGE("open %s failed", DEV_GRAPHICS);
        return 2;
    }

    fd = ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);

    if(fd < 0){
        LOGE("ioctl failed");
        return 2;
    }



    lua_pushnumber(L, fb_var.xres);
    lua_pushnumber(L, fb_var.yres);

    close(fd);
    return 2;                         /*返回0个值，所以为0*/
}


static const struct luaL_Reg libs[] = {
        {"sleep", system_sleep},
        {"getScreenSize", system_getScreenSize},
        {"touch", system_touch},
        {NULL, NULL}  /*the end*/
};

int luaopen_system (lua_State *L){
    /*注册lib， 上面luaopen_名称 跟下面注册的名称要一致, 还要和编译的.so文件名一致*/
    luaL_register(L, "system", libs);

    return 1;
}
//cat /dev/graphics/fb0 > /storage/emulated/0/DCIM/screen.raw
//ls -l /dev/graphics/fb0