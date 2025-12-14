#ifndef __CMD_H__
#define __CMD_H__

#include <time.h>

typedef enum {
    cmd_request,
    cmd_response
} cmd_type;

typedef enum {
    cmd_login,
    cmd_logout,
    cmd_msg
} cmd_id;

typedef struct cmd {
    cmd_type type;
    cmd_id id;
    time_t time;

    union {
        struct login_data {
            char username[50];
            bool listener;
        } login;

        struct logout_data {
            char username[50];
        } logout;

        struct msg_data {
            char username[50];
            char msg[200];
        } msg;
    } data;
} cmd;

#endif