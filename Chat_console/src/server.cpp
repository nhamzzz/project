#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 50
#define BUFFER_SIZE 1024

// Struct to store client information
typedef struct {
    int socket;
    struct sockaddr_in address;
    int active;
    char name[50];   // NEW: client name
    int named;       // NEW: has the client sent their name?
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Broadcast a message to all clients
void broadcast(char *msg, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].socket != sender_sock) {
            send(clients[i].socket, msg, strlen(msg), 0);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

// Thread for each client
void* client_handler(void *arg) {
    Client *cli = (Client*) arg;
    char buffer[BUFFER_SIZE];

    printf("Client connected: %d\n", cli->socket);

    // 1️⃣ Wait for the client's name first
    ssize_t name_size = recv(cli->socket, buffer, sizeof(buffer)-1, 0);

    if (name_size <= 0) {
        printf("Client disconnected before sending name.\n");
        cli->active = 0;
        close(cli->socket);
        return NULL;
    }

    buffer[name_size] = '\0';
    strcpy(cli->name, buffer);
    cli->named = 1;

    printf("Client %d registered as: %s\n", cli->socket, cli->name);

    // Notify others
    char joinMsg[BUFFER_SIZE];
    sprintf(joinMsg, "[SERVER] %s joined the chat.\n", cli->name);
    broadcast(joinMsg, cli->socket);

    // 2️⃣ Now start normal chat loop
    while (1) {
        ssize_t read_size = recv(cli->socket, buffer, sizeof(buffer)-1, 0);

        if (read_size <= 0) {
            printf("%s disconnected.\n", cli->name);
            cli->active = 0;

            char leaveMsg[BUFFER_SIZE];
            sprintf(leaveMsg, "[SERVER] %s left the chat.\n", cli->name);
            broadcast(leaveMsg, cli->socket);

            close(cli->socket);
            return NULL;
        }

        buffer[read_size] = '\0';

        // Format: "[Name]: message"
        char formatted[BUFFER_SIZE + 100];
        sprintf(formatted, "[%s]: %s", cli->name, buffer);

        broadcast(formatted, cli->socket);
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Initialize the clients array
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].active = 0;
        clients[i].named = 0;
    }

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    // Server address info
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Chat server listening on port %d...\n", PORT);

    // Accept loop
    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Find free client slot
        pthread_mutex_lock(&clients_mutex);
        int index = -1;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            printf("Server full! Rejecting...\n");
            close(new_socket);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }

        // Setup client data
        clients[index].socket = new_socket;
        clients[index].address = client_addr;
        clients[index].active = 1;
        clients[index].named = 0;

        pthread_mutex_unlock(&clients_mutex);

        // Create client thread
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, &clients[index]);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
