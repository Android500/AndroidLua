//
// Created by Administrator on 2018/1/16.
//

#ifndef ANDROIDLUA_INPUT_EVENT_H
#define ANDROIDLUA_INPUT_EVENT_H
int init_uinput_dev();
void mouse_move(int dx, int dy);
int write_click_event(int x, int y);
int write_back_event();
int write_volume_up();
int write_volume_down();
/**
 * 发送home键指令
 * */
int write_home_event();
int write_home_page_event();
/**
 * 发送菜单键命令
 * */
int write_menu_event();
/**销毁虚拟输入设备*/
int destroy();

int touchDown(int x, int y);
int touchScroll(int x, int y);
int touchUp(int x, int y);

int write_touch_button(int x, int y, int down);
#endif //ANDROIDLUA_INPUT_EVENT_H
