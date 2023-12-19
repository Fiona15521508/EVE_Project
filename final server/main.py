import socket
from servo import Servo

PORT = 8081
# HOST = '127.0.0.1'  # TODO: Change this to the IP address of the computer running the server
HOST = '192.168.103.7'


def initServer():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (HOST, PORT)
    server_socket.bind(server_address)
    server_socket.listen(1)

    print("Waiting for connection...")
    client_socket, client_address = server_socket.accept()
    print(f"Connection from {client_address} has been established!")

    return server_socket, client_socket


def main():
    # Initialize the serve
    server_socket, client_socket = initServer()
    # Initialize the servo
    servo = Servo(12)

    try:
        while True:
            # Receive data from client
            data = client_socket.recv(1024)
            if not data:
                print("Client has disconnected!")
                break

            # Decode the data
            data = data.decode('utf-8')
            #if data have \n,remove it
            data = data.replace('\n','')
            print(f"Received data: {data}")
            
            if int(data) >= 0 and int(data) <= 73:
                # Set the servo angle
                servo.setAngle(int(data))
            else:
                print("Invalid data!")
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

