#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1000];
    FILE *file;

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    listen(server_socket, 3);
    printf("Waiting for incoming connections...\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Client connected.\n");

    file = fopen("message.txt", "r");
    if (file == NULL) {
        printf("Failed to open message.txt\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    send(client_socket, buffer, strlen(buffer), 0);
    printf("Sent to client: %s\n", buffer);

    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);

    file = fopen("result.txt", "w");
    if (file == NULL) {
        printf("Failed to open result.txt\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    fprintf(file, "%s\n", buffer);
    fclose(file);

    printf("Saved result to result.txt: %s\n", buffer);

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
