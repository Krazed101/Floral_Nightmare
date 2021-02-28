#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFFER_SIZE 2000

int main(char* argv[], int argc)
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];
    char recv_msg[BUFFER_SIZE];
    int recv_size;

    printf("\nInitializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) !=  0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        goto cleanup;
    }
    printf("Initialized.");

    if((s=socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d\n", WSAGetLastError());
        goto cleanup;
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8080 );

    //Connect to remote server
    if ( connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connection Error\n");
        goto cleanup;
    }
    printf("Connected\n");

    while( TRUE )
    {

        //Send some data
        fgets(message,BUFFER_SIZE,stdin);
        if (send(s, message, strlen(message), 0) < 0)
        {
            printf("Send failed\n");
            goto cleanup;
        }
        printf("Data Sent\n");

        //Recv some data
        if(( recv_size = recv(s, recv_msg, BUFFER_SIZE, 0)) == SOCKET_ERROR)
        {
            printf("Recv failed\n");
            goto cleanup;
        }
        printf("Data Recieved\n");
        recv_msg[recv_size] = '\0';
        printf("%s\n",recv_msg);
    }

cleanup:
    closesocket(s);
    WSACleanup();

    return 0;
}