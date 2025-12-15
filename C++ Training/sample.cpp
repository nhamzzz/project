#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctime>

#include "cmd.h"

/* =======================
   Global socket list
   (NO mutex – unsafe by design)
   ======================= */

std::vector<int> listeners;

/* =======================
   Utilities
   ======================= */

std::string now() {
    char buf[64];
    time_t t = time(nullptr);
    strftime(buf, sizeof(buf), "%F %T", localtime(&t));
    return buf;
}

bool send_cmd(int s, const cmd& c) {
    return send(s, &c, sizeof(cmd), 0) == sizeof(cmd);
}

bool recv_cmd(int s, cmd& c) {
    return recv(s, &c, sizeof(cmd), MSG_WAITALL) == sizeof(cmd);
}

/* =======================
   Broadcast (NO mutex)
   ======================= */

void broadcast(const cmd& c, int sender) {
    for (int s : listeners) {
        if (s != sender) {
            send_cmd(s, c);
        }
    }

    std::cout << "[" << now() << "] Broadcast from "
              << c.data.msg.username << "\n";
}

/* =======================
   Client thread
   ======================= */

void* client_thread(void* arg) {
    int sock = *(int*)arg;
    delete (int*)arg;

    cmd login{};
    if (!recv_cmd(sock, login) || login.id != cmd_login) {
        close(sock);
        return nullptr;
    }

    std::string user = login.data.login.username;
    bool is_listener = login.data.login.listener;

    if (is_listener) {
        listeners.push_back(sock);
        std::cout << "[" << now() << "] "
                  << user << " listener connected\n";
    } else {
        std::cout << "[" << now() << "] "
                  << user << " sender connected\n";
    }

    cmd c{};
    while (recv_cmd(sock, c)) {

        switch (c.id) {

        case cmd_msg:
            std::cout << "[" << now() << "] "
                      << c.data.msg.username << ": "
                      << c.data.msg.msg << "\n";
            broadcast(c, sock);
            break;

        case cmd_logout:
            std::cout << "[" << now() << "] "
                      << user << " logged out\n";
            close(sock);
            return nullptr;

        default:
            break;
        }
    }

    close(sock);
    return nullptr;
}

/* =======================
   MAIN
   ======================= */

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9000);

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 10);

    std::cout << "Server listening on port 9000\n";

    while (true) {
        int client = accept(server, nullptr, nullptr);

        int* sock = new int(client);
        pthread_t tid;
        pthread_create(&tid, nullptr, client_thread, sock);
        pthread_detach(tid);
    }
}
