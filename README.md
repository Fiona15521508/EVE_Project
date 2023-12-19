# Introduction

This is a project to implement wifi server and client using C language (python).software is developed in Linux and Windows environment.

# How to use

IMPORTANT: You need to change the IP address in the code to your own IP address.
IMPORTANT: launch the server first, then the client.

## Server

### Windows

```
1. gcc ./server.c -o server.exe -lws2_32
2. ./server.exe
```

### Linux

```
1. gcc ./server.c -o server
2. ./server
```

### Final

```
1. python3 ./main.py
```

## Client

```
1. gcc ./client.c -o client.exe -lws2_32
2. ./client.exe
```



## License

Licensed under the MIT License.
