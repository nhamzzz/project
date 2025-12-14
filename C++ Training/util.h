#include "cmd.h"
#include <unistd.h>

bool send_cmd(int sock, const cmd& c) {
    return send(sock, &c, sizeof(cmd), 0) == sizeof(cmd);
}

bool recv_cmd(int sock, cmd& c) {
    return recv(sock, &c, sizeof(cmd), MSG_WAITALL) == sizeof(cmd);
}