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
    std::cout << "Username: ";
    std::getline(std::cin, user);

    int sock = connect_to_server();

    cmd login{};
    login.type = cmd_request;
    login.id = cmd_login;
    login.time = time(nullptr);
    strcpy(login.data.login.username, user.c_str());
    login.data.login.listener = false;

    send_cmd(sock, login);

    std::cout << "Type messages or /logout\n> ";

    while (true) {
        std::string text;
        std::getline(std::cin, text);

        if (text == "logout") {
            cmd c{};
            c.type = cmd_request;
            c.id = cmd_logout;
            strcpy(c.data.logout.username, user.c_str());
            send_cmd(sock, c);
            close(sock);
            break;
        }

        cmd m{};
        m.type = cmd_request;
        m.id = cmd_msg;
        strcpy(m.data.msg.username, user.c_str());
        strncpy(m.data.msg.msg, text.c_str(), sizeof(m.data.msg.msg)-1);
        send_cmd(sock, m);

        std::cout << "> ";
    }
}
