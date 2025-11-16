// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    const char* sourceFile = argv[1];
    FILE* src = fopen(sourceFile, "rb");
    if (!src) {
        perror("Source file");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Connect to localhost

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        write(sock, buffer, bytesRead);
    }

    printf("File sent successfully.\n");

    fclose(src);
    close(sock);

    return 0;
}
