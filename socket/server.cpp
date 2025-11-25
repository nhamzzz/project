#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Reuse address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server running... waiting for clients.\n");

    // 🔁 Keep accepting clients forever
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue; // continue loop instead of exit
        }

        printf("Client connected!\n");

        // Open new file for each client
        FILE* dest = fopen("test1.txt", "a");
        if (!f) {
            perror("File open error");
            close(new_socket);
            continue;
        }

        // Receive data loop
        while ((valread = read(new_socket, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, valread, dest);
        }

        printf("Client disconnected.\n");

        fclose(f);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
