#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <time.h>

#include "cmd.h"
#include "util.h"

/* =======================
   Connect to server
   ======================= */

int connect_to_server() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

/* =======================
   Prompt username
   ======================= */

std::string prompt_username() {
    std::string username;
    while (true) {
        std::cout << "Enter username (same as sender): ";
        std::getline(std::cin, username);
        if (!username.empty() && username.size() < 50)
            return username;
    }
}

/* =======================
   MAIN
   ======================= */

int main() {
    std::string username = prompt_username();
    int sock = connect_to_server();
    if (sock < 0)
        return 1;

    /* Login as listener */
    cmd login{};
    login.type = cmd_request;
    login.id   = cmd_login;
    login.time = time(nullptr);
    strcpy(login.data.login.username, username.c_str());
    login.data.login.listener = true;

    send_cmd(sock, login);

    std::cout << "Logged in as listener (" << username << ")\n";
    std::cout << "Waiting for messages...\n\n";

    /* Receive messages */
    cmd c{};
    while (recv_cmd(sock, c)) {

        switch (c.id) {

        case cmd_msg:
            std::cout << "[" << c.data.msg.username << "]: "
                      << c.data.msg.msg << "\n";
            break;

        case cmd_logout:
            std::cout << "Server: logged out\n";
            goto done;

        default:
            break;
        }
    }

done:
    close(sock);
    std::cout << "Disconnected from server\n";
    return 0;
}
