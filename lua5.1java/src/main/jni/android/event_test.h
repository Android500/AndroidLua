//
// Created by Administrator on 2018/1/17.
//

#ifndef ANDROIDLUA_EVENT_TEST_H
#define ANDROIDLUA_EVENT_TEST_H

int setup_uinput_device();
int destroy_uinput_device();
int write_touch_move(int x, int y);
int write_touch_hide(void);
int write_touch_button(int x, int y, int down);
int write_key(int key, int down);
int main();
#endif //ANDROIDLUA_EVENT_TEST_H
