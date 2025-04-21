#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

void encryption(char text[], int key) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z')
            text[i] = (text[i] - 'A' + key) % 26 + 'A';
        else if (text[i] >= 'a' && text[i] <= 'z')
            text[i] = (text[i] - 'a' + key) % 26 + 'a';
        else if (text[i] >= '0' && text[i] <= '9')
            text[i] = (text[i] - '0' + key) % 10 + '0';
        else if (text[i] >= 32 && text[i] <= 126)
            text[i] = (text[i] - 32 + key) % 95 + 32;
    }
}

void decryption(char text[], int key) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z')
            text[i] = (text[i] - 'A' - key + 26) % 26 + 'A';
        else if (text[i] >= 'a' && text[i] <= 'z')
            text[i] = (text[i] - 'a' - key + 26) % 26 + 'a';
        else if (text[i] >= '0' && text[i] <= '9')
            text[i] = (text[i] - '0' - key + 10) % 10 + '0';
        else if (text[i] >= 32 && text[i] <= 126)
            text[i] = (text[i] - 32 - key + 95) % 95 + 32;
    }
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char text[1000];
    int key;
    char choice;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("192.168.137.1"); // change to server's IP
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Connected to server.\n");

    recv(sock, text, sizeof(text), 0);
    text[strcspn(text, "\n")] = '\0';

    printf("Received from server: %s\n", text);

    printf("Enter the key for the shift: ");
    scanf("%d", &key);

    if (key < 0) {
        printf("Key must be positive.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    key = key % 95;

    printf("Enter E for Encryption or D for Decryption: ");
    scanf(" %c", &choice);

    if (choice == 'E' || choice == 'e') {
        encryption(text, key);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'D' || choice == 'd') {
        decryption(text, key);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    send(sock, text, strlen(text), 0);
    printf("Result sent to server.\n");

    closesocket(sock);
    WSACleanup();
    return 0;
}