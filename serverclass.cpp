#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>
#include "mydata.h"
#include "struct.h"
#include "serverclass.h"
#define IP "127.0.0.1"
#define PORT 9090
using namespace std;


// Server class to create socket ,binding and to send and receive the commands

server::server()
{}
server::~server()
{}

  int server::sock_bind_listen() {
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }
 if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
        //inet_addr(IP);  // change to your machine ip address
    address.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 5) < 0) {
      perror("Listen");
      exit(EXIT_FAILURE);
    }
    return sock_fd;
  }

  int server:: myaccept() {
    newsock_fd =
        accept(sock_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  
    // adding ip address and port number to map
    s += "IP_ADDRESS: ";
    s += inet_ntoa(server::address.sin_addr);
    s += "  PORT_NUMBER: " + to_string(address.sin_port);
    active_connections.insert(make_pair(newsock_fd, s));
    s = "";
    return newsock_fd;
    
  }             



