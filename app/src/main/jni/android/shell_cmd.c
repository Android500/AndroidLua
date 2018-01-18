//
// Created by Administrator on 2018/1/17.
//


#include <stdio.h>
#include <stdlib.h>
#include "shell_cmd.h"
#include "androidlib.h"

int input_text(char *content){

    int length = sizeof(CMD_INPUT_TEXT) + sizeof(content) + sizeof(USER);
    char *cmd = malloc(sizeof(char) * length);
    sprintf(cmd, "%s%s %s", CMD_INPUT_TEXT, content, USER);
    LOGE("%s:%s", __FUNCTION__, cmd);
    return system(cmd);
}


int run_app(char *action){

    int length = sizeof(CMD_RUN_APP) + sizeof(action) + sizeof(USER);
    char *cmd = malloc(sizeof(char) * length);
    sprintf(cmd, "%s%s %s", CMD_RUN_APP, action, USER);
    LOGE("%s:%s", __FUNCTION__, cmd);
    return system(cmd);
}

int press_home(){

    int length = sizeof(CMD_HOME) + sizeof(USER);
    char *cmd = malloc(sizeof(char) * length);
    sprintf(cmd, "%s %s", CMD_HOME, USER);

    LOGE("%s:%s", __FUNCTION__, cmd);

    system(cmd);
}