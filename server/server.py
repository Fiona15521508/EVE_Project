import socket

PORT = 8081
HOST = '127.0.0.1'  # TODO: Change this to the IP address of the computer running the server


def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (HOST, PORT)
    server_socket.bind(server_address)
    server_socket.listen(1)

    print("Waiting for connection...")
    client_socket, client_address = server_socket.accept()
    print(f"Connection from {client_address} has been established!")

    try:
        while True:
            # Receive data from client
            data = client_socket.recv(1024)
            if not data:
                print("Client has disconnected!")
                break

            print(f"Received data from client: {data.decode('utf-8')}", end="")

            # Send ACK to client
            response = "ACK OK"
            client_socket.send(response.encode('utf-8'))

    except KeyboardInterrupt:
        print("Server has been stopped by the user!")
    except ConnectionResetError:
        print("Client has disconnected!")
    finally:
        # Close the connection
        client_socket.close()
        server_socket.close()
        print("Server has been stopped!")


if __name__ == "__main__":
    main()
