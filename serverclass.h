#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <map>
using namespace std;
class server
{
    public:
    int sock_fd,newsock_fd,opt=1;
    string s="";
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    server();
    ~server();
    int sock_bind_listen();
    int myaccept();
};
// map for keeping track of active conections
extern map<int, string> active_connections; 
#endif
