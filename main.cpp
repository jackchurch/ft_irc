// #include "main.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> // this is the library for the Internet Protocol. 
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

// addrinfo struct contains...
//     struct addrinfo {
//         int              ai_flags;
//         int              ai_family;
//         int              ai_socktype;
//         int              ai_protocol;
//         socklen_t        ai_addrlen;
//         struct sockaddr *ai_addr;
//         char            *ai_canonname;
//         struct addrinfo *ai_next;
//     };

void    error(const char *message)
{
    perror(message);
    exit(1);
}

// void    *get_in_addr(struct sock_addr* sa)
// {
//        return &(((struct sockaddr_in*)sa)->sin_addr);
// }

// int main(int argc, char* argv[])
int main(void)
{
    int         socket_fd = 0;
    int         port;
    int         bind_return;
    int         client_accept_fd;
    socklen_t   client_address_lenth;

    struct  addrinfo    hints;
    struct  sockaddr_in serverAddress;
    struct  sockaddr_in clientAddress;

    // if (argc != 2)
    //     error("Argc count error.");
    // if (!(port = atoi(argv[1])))
    //     error("Check port number is a number.");
    // if (port < 1024 || port > 49152)
    //     error("Port number out of range.");
    port = 5555;
    memset(&hints, 0, sizeof(hints));
    // int socket(int domain, int type, int protocol)
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error("socket() failed to create");
    else
        std::cout << "socket() is sucessful. socket_fd is " << socket_fd << std::endl;

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    serverAddress.sin_family = AF_INET;
     // htons does a conversion from small to big endian. 
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    // int bind(int socketfd, strcut socketaddr *my_addr, it addrlen)
    if ((bind_return = bind(socket_fd,(struct sockaddr *)&serverAddress, sizeof(serverAddress))) != 0)
        error("Binding error");
    else
        std::cout << "bind() is successful." << std::endl;
    std::cout << "Port is set to " << port << std::endl;

    // SOMAXCONN is maximum connections per Processor/OS. Intel/MacOS is 128. 
    listen(socket_fd, SOMAXCONN); 


    while (1)
    {
        // socklen_t 32 bit interger that is opaque to me. 
        client_address_lenth = sizeof(clientAddress);
        if ((client_accept_fd = accept(socket_fd, (struct sockaddr *)&clientAddress, &client_address_lenth)) == -1)
            error("Accept() error.");
      


        // The function fcntl() is set in stone by the subject PDF inplace of the write() function. 
        // F_SETFL            Set descriptor status flags to arg.
        // O_NONBLOCK   Non-blocking I/O; if no data is available to a read call, or if a write operation would block, the read or write call returns -1 with the error EAGAIN.
        if (fcntl(client_accept_fd, F_SETFL, O_NONBLOCK) == -1)
            error("fcntl() error");
        else
            std::cout << "Set non blocking" << std::endl;
        send(client_accept_fd, "Hello\n", 6, 0);
    }
}

