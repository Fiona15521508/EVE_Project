#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#define PORT 8081
#define SERVER_ADDR "192.168.8.7" //todo:  server ip for the PROJECT
// #define SERVER_ADDR "127.0.0.1" // todo:  server ip for test just in windows localhost

SOCKET sock = INVALID_SOCKET;

/**
 * @brief This function initializes the client.
 *
 * @return int : 0 if success, 1 if error
 */
int clientInit()
{
    WSADATA wsaData;

    // initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("[ERROR] WSAStartup failed.\n");
        return 1;
    }

    // create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("[ERROR] Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // change IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0)
    {
        printf("[ERROR] Invalid address/ Address not supported.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("[ERROR] Connection Failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    return 0;
}

/**
 * @brief this function sends data to the server.
 *
 * @param data : data to send
 * @return int : 0 if success, 1 if error
 */
int clientSendData(const char* data)
{
    if (send(sock, data, strlen(data), 0) == SOCKET_ERROR)
    {
        printf("[ERROR] Send failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}

/**
 * @brief This function receives data from the server.
 *
 * @param buffer ： buffer to store the received data
 * @param bufsize ： buffer size
 * @return int  ： 0 if success, 1 if error
 */
int clientReceiveData(char* buffer, int bufsize)
{
    int valread = recv(sock, buffer, bufsize, 0);
    if (valread == SOCKET_ERROR)
    {
        printf("[ERROR] Receive failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    buffer[valread] = '\0'; // 确保字符串以 null 结尾
    return 0;
}

int main1()
{
    if (clientInit() != 0)
    {
        return 1;
    }

    char data_to_send[1024];
    char buffer[1024];
    bool running = true;

    while (running)
    {
        printf("[INFO] Message to send: ");
        fgets(data_to_send, sizeof(data_to_send), stdin);

        // check if user wants to quit
        if (strncmp(data_to_send, "exit", 4) == 0)
        {
            running = false;
            break;
        }
        // send data to server
        if (clientSendData(data_to_send) != 0)
        {
            break;
        }
        // receive data from server
        if (clientReceiveData(buffer, sizeof(buffer)) != 0)
        {
            break;
        }
        printf("[INFO] Client received: %s\n", buffer);
    }

    // close socket and cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}
