#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int main(int argc,char *argv[])
{
    if(argc!=3 ||strcmp(argv[1],"-port")!=0)
    {
        printf("error,current arg=myServer -port <port_number>\n");
        return 1;
    }

    char *endptr;
    long port = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0' || port <= 0 || port > 65535)
    {
        printf("Invalid port number\n");
        return 1;
    }
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverIPPort,clientIPPort;
    char buffer[BUFFER_SIZE];
    int clientAddrSize=sizeof(clientIPPort);
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) 
    {
        printf("Error creating socket\n");
        WSACleanup();
        return 1;
    }

    serverIPPort.sin_family = AF_INET;
    serverIPPort.sin_addr.s_addr = INADDR_ANY;
    serverIPPort.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *)&serverIPPort, sizeof(serverIPPort)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }   

    if(listen(serverSocket, 1)==SOCKET_ERROR)
    {
        printf("listen failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Server listening on port %d\n", port);

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientIPPort, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        printf("Error accepting connection\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Client connected from %s\n", inet_ntoa(clientIPPort.sin_addr));

    while (1) 
    {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) 
        {
            printf("Client disconnected\n");
            break;
        }
        buffer[bytesReceived] = '\0';
        printf("Client: %s\n", buffer);
    }

    
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;



}