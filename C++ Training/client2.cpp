#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <time.h>

#include "cmd.h"
#include "util.h"

int connect_to_server() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    connect(s, (sockaddr*)&a, sizeof(a));
    return s;
}

int main() {
    std::string user;
    std::cout << "Username (same as sender): ";
    std::cout << "Start listening to messages... ";
    std::getline(std::cin, user);

    int sock = connect_to_server();

    cmd login{};
    login.type = cmd_request;
    login.id = cmd_login;
    login.time = time(nullptr);
    strcpy(login.data.login.username, user.c_str());
    login.data.login.listener = true;

    send_cmd(sock, login);

    cmd c{};
    while (recv_cmd(sock, c)) {
        if (c.id == cmd_msg) {
            std::cout << "[" << c.data.msg.username << "]: "
                      << c.data.msg.msg << "\n";
        } else if (c.id == cmd_logout) {
            std::cout << c.data.logout.username << " logged out\n";
            break;
        }
    }

    close(sock);
}
