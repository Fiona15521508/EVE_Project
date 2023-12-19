#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#define PORT 8081

SOCKET server_fd = INVALID_SOCKET;

/**
 * @brief This function initializes the server.
 *
 * @return int : 0 if success, 1 if error
 */
int serverInit()
{
    WSADATA wsaData;
    struct sockaddr_in address;
    int opt = 1;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("[ERROR] WSAStartup failed.\n");
        return 1;
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("[ERROR] Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)))
    {
        printf("[ERROR] Setsockopt failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        printf("[ERROR] Bind failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen on the socket
    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        printf("[ERROR] Listen failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    return 0;
}

/**
 * @brief This function accepts a connection from a client.
 *
 * @param client_socket  : pointer to the client socket
 * @return int         : 0 if success, 1 if error
 */
int serverAcceptConnection(SOCKET *client_socket)
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    *client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (*client_socket == INVALID_SOCKET)
    {
        printf("[ERROR] Accept failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    return 0;
}

/**
 * @brief   This function receives data from the client.
 *
 * @param client_socket  : client socket
 * @param buffer      : buffer to store the received data
 * @param bufsize   : buffer size
 * @return int
 */
int serverReceiveData(SOCKET client_socket, char *buffer, int bufsize)
{
    int valread = recv(client_socket, buffer, bufsize, 0);
    if (valread == SOCKET_ERROR)
    {
        printf("[ERROR] Receive failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    buffer[valread] = '\0'; // Ensure null termination
    return 0;
}

/**
 * @brief   This function sends data to the client.
 *
 * @param client_socket  : client socket
 * @param data    : data to send
 * @return int   : 0 if success, 1 if error
 */
int serverSendData(SOCKET client_socket, const char *data)
{
    if (send(client_socket, data, strlen(data), 0) == SOCKET_ERROR)
    {
        printf("[ERROR] Send failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}

int main()
{
    char sendBuffer[1024];
    if (serverInit() != 0)
    {
        return 1;
    }

    printf("[INFO] Server is running on port %d\n", PORT);

    while (1)
    {
        SOCKET new_socket;
        if (serverAcceptConnection(&new_socket) != 0)
        {
            continue; // Continue to the next iteration if accept fails
        }

        char receiveBuffer[1024];
        if (serverReceiveData(new_socket, receiveBuffer, sizeof(receiveBuffer)) == 0)
        {
            printf("[INFO] Server Received: %s\n", receiveBuffer);
            int fakedata = 100; // TODO: Replace with actual data IN THE PROJECT
            snprintf(sendBuffer, sizeof(sendBuffer), "OK %d", fakedata);
            serverSendData(new_socket, sendBuffer);
            printf("[INFO] Server Sent: %s\n", sendBuffer);
        }

        closesocket(new_socket); // Close client socket
    }

    // Clean up
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
