#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#include "cmd.h"

/* =======================
   Listener registry
   ======================= */

struct Listener {
    int sock;
    std::string username;
};

std::vector<Listener> listeners;

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
   Broadcast
   ======================= */

void broadcast(const cmd& c, int exclude = -1) {
    std::vector<std::string> delivered;

    for (auto& l : listeners) {
        if (l.sock != exclude) {
            send_cmd(l.sock, c);
            delivered.push_back(l.username);
        }
    }

    std::cout << "[" << now() << "] Broadcast from "
              << c.data.msg.username;

    if (!delivered.empty()) {
        std::cout << " → delivered to: ";
        for (size_t i = 0; i < delivered.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << delivered[i];
        }
    } else {
        std::cout << " → no listeners";
    }

    std::cout << "\n";
}

/* =======================
   Remove listener
   ======================= */

void remove_listener(const std::string& user) {
    for (auto it = listeners.begin(); it != listeners.end(); ++it) {
        if (it->username == user) {
            close(it->sock);
            listeners.erase(it);
            break;
        }
    }
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
        listeners.push_back({sock, user});
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

        case cmd_logout: {
            std::cout << "[" << now() << "] "
                      << user << " logged out\n";

            /* create logout broadcast */
            cmd out{};
            out.type = cmd_response;
            out.id   = cmd_logout;
            out.time = time(nullptr);
            strcpy(out.data.logout.username, user.c_str());

            broadcast(out);
            remove_listener(user);

            close(sock);
            return nullptr;
        }

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
