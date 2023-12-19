#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081

int server_fd = -1;

/**
 * @brief This function initializes the server.
 *
 * @return int : 0 if success, 1 if error
 */
int serverInit()
{
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        return 1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // Listen on the socket
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    return 0;
}

/**
 * @brief This function accepts a connection from a client.
 *
 * @param client_socket : Pointer to the client socket file descriptor
 * @return int : 0 if success, 1 if error
 */
int serverAcceptConnection(int *client_socket)
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    *client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (*client_socket < 0)
    {
        perror("accept");
        return 1;
    }

    return 0;
}

/**
 * @brief This function receives data from the client.
 *
 * @param client_socket : Client socket file descriptor
 * @param buffer        : Buffer to store the received data
 * @param bufsize       : Size of the buffer
 * @return int : 0 if success, 1 if error
 */
int serverReceiveData(int client_socket, char *buffer, int bufsize)
{
    int valread = read(client_socket, buffer, bufsize);
    if (valread < 0)
    {
        perror("read");
        return 1;
    }
    buffer[valread] = '\0'; // Ensure null termination
    return 0;
}

int serverSendData(int client_socket, const char *data)
{
    if (write(client_socket, data, strlen(data)) < 0)
    {
        perror("write");
        return 1;
    }
    return 0;
}

int main()
{
    char sendBuffer[1024] = {0};
    if (serverInit() != 0)
    {
        return 1;
    }

    printf("Server is running on port %d\n", PORT);

    while (1)
    {
        int new_socket;
        if (serverAcceptConnection(&new_socket) != 0)
        {
            continue; // Continue to the next iteration if accept fails
        }

        char receiveBuffer[1024];
        if (serverReceiveData(new_socket, receiveBuffer, sizeof(receiveBuffer)) == 0)
        {
            if (strcmp(receiveBuffer, "1") == 0)
            {
                servoforward();
            }
            if (strcmp(receiveBuffer, "0") == 0)
            {
                servobarkword();
            }
            printf("Received: %s\n", receiveBuffer);
            int fakedata = 100; // TODO: Replace with actual data IN THE PROJECT
            snprintf(sendBuffer, sizeof(sendBuffer), "OK %d", fakedata);
            serverSendData(new_socket, sendBuffer);
            printf("[INFO] Server Sent: %s\n", sendBuffer);
        }

        close(new_socket); // Close client socket
    }

    // Clean up
    close(server_fd);

    return 0;
}
