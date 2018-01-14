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

#include <android/log.h>

#define TAG    "lua_android_support" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

/*simple sleep*/
static int system_sleep (lua_State *L){
    long secs = lua_tointeger(L, -1); /*获取参数*/
    sleep(secs);
    return 0;                         /*返回0个值，所以为0*/
}

/*simple sleep*/
static int system_getScreenSize(lua_State *L){

    struct fb_var_screeninfo fb_var;
    int fd = open("/dev/graphics/fb0", O_RDONLY);

    LOGD("########## w = %d", fd);

    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);


    int w = fb_var.xres;

    LOGD("########## w = %d", w);

    lua_pushnumber(L,  500);
    lua_pushnumber(L,     fb_var.yres);

    close(fd);
    return 2;                         /*返回0个值，所以为0*/
}


static const struct luaL_Reg libs[] = {
        {"sleep", system_sleep},
        {"getScreenSize", system_getScreenSize},
        {NULL, NULL}  /*the end*/
};

int luaopen_system (lua_State *L){
    /*注册lib， 上面luaopen_名称 跟下面注册的名称要一致, 还要和编译的.so文件名一致*/
    luaL_register(L, "system", libs);

    return 1;
}
//cat /dev/graphics/fb0 > /storage/emulated/0/DCIM/screen.raw
//ls -l /dev/graphics/fb0