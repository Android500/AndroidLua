//
// Created by Administrator on 2018/1/16.
//

#ifndef ANDROIDLUA_INPUT_EVENT_H
#define ANDROIDLUA_INPUT_EVENT_H
int init_uinput_dev();
void mouse_move(int dx, int dy);
void write_click_events(int x, int y);
void send_keyboard_event(int keycode);
void mouse_left_click();
void send_a_button();
#endif //ANDROIDLUA_INPUT_EVENT_H
