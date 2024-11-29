#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[1], "-ip") != 0 || strcmp(argv[3], "-port") != 0) 
    {
        printf("Usage: myClient -ip <ip_address> -port <port_number>\n");
        return 1;
    }

    const char *ip = argv[2];
    int port = atoi(argv[4]);
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) 
    {
        printf("Error creating socket\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        printf("Connection failed\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Connected to server\n");

    while (1) 
    {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  

        if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) 
        {
            printf("Connection lost\n");
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}