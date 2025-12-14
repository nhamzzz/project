#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctime>

#include "cmd.h"

/* =======================
   Client structure
   ======================= */

struct Client {
    int sender   = -1;
    int listener = -1;
    pthread_t thread{};
    bool running = false;
};

std::unordered_map<std::string, Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

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
   Logout notification
   ======================= */

void notify_listener_logout(const std::string& user) {
    if (!clients.count(user))
        return;

    int listener = clients[user].listener;
    if (listener == -1)
        return;

    cmd c{};
    c.type = cmd_response;
    c.id   = cmd_msg;
    c.time = time(nullptr);

    strcpy(c.data.msg.username, "SERVER");
    snprintf(c.data.msg.msg,
             sizeof(c.data.msg.msg),
             "You have been logged out.");

    send_cmd(listener, c);
}

/* =======================
   Broadcast
   ======================= */

void broadcast(const cmd& message) {
    std::vector<std::string> delivered;

    for (auto& [user, client] : clients) {
        if (client.listener != -1) {
            send_cmd(client.listener, message);
            delivered.push_back(user);
        }
    }

    std::cout << "[" << now() << "] Broadcast from "
              << message.data.msg.username;

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
   Client worker
   ======================= */

void* client_worker(void* arg) {
    std::string user = *(std::string*)arg;
    delete (std::string*)arg;

    while (true) {
        pthread_mutex_lock(&clients_mutex);

        if (!clients.count(user)) {
            pthread_mutex_unlock(&clients_mutex);
            break;
        }

        int sock = clients[user].sender;
        pthread_mutex_unlock(&clients_mutex);

        cmd c{};
        if (!recv_cmd(sock, c))
            break;

        switch (c.id) {

        case cmd_msg:
            std::cout << "[" << now() << "] "
                      << user << ": "
                      << c.data.msg.msg << "\n";

            pthread_mutex_lock(&clients_mutex);
            broadcast(c);
            pthread_mutex_unlock(&clients_mutex);
            break;

        case cmd_logout:
            std::cout << "[" << now() << "] "
                      << user << " requested logout\n";

            pthread_mutex_lock(&clients_mutex);
            notify_listener_logout(user);
            pthread_mutex_unlock(&clients_mutex);

            goto cleanup;

        default:
            break;
        }
    }

cleanup:
    pthread_mutex_lock(&clients_mutex);

    if (clients.count(user)) {
        if (clients[user].sender != -1)
            close(clients[user].sender);

        if (clients[user].listener != -1)
            close(clients[user].listener);

        clients.erase(user);
    }

    pthread_mutex_unlock(&clients_mutex);

    std::cout << "[" << now() << "] "
              << "User '" << user << "' disconnected\n";

    return nullptr;
}

/* =======================
   Handle new connections
   ======================= */

void handle_connection(int sock) {
    cmd c{};
    if (!recv_cmd(sock, c) || c.id != cmd_login) {
        close(sock);
        return;
    }

    std::string user = c.data.login.username;
    bool is_listener = c.data.login.listener;

    pthread_mutex_lock(&clients_mutex);
    auto& cl = clients[user];

    if (is_listener && cl.listener != -1) {
        pthread_mutex_unlock(&clients_mutex);
        close(sock);
        return;
    }

    if (!is_listener && cl.sender != -1) {
        pthread_mutex_unlock(&clients_mutex);
        close(sock);
        return;
    }

    if (!is_listener) {
        cl.sender = sock;

        if (!cl.running) {
            cl.running = true;
            auto* arg = new std::string(user);
            pthread_create(&cl.thread, nullptr, client_worker, arg);
            pthread_detach(cl.thread);
        }

        std::cout << "[" << now() << "] "
                  << user << " sender connected\n";
    }
    else {
        cl.listener = sock;
        std::cout << "[" << now() << "] "
                  << user << " listener connected\n";
    }

    pthread_mutex_unlock(&clients_mutex);
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
        handle_connection(client);
    }
}
