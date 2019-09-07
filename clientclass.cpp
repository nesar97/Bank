#include <arpa/inet.h>
#include <readline/history.h>
#include <readline/readline.h> 
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#define PORT 9090
#define IP "127.0.0.1"   //change accordingly
using namespace std;

class client {
 public:
  int sock_fd;
  string a;
  char buffer[50000] = {0};
  struct sockaddr_in serv_addr;
  char *line;

  void create_socket() {
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      cout << " Socket creation error \n";
      exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);
  }

  void connection() {
    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
      cout << "\nInvalid address/ Address not supported \n";
      exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) {
      cout << "\nConnection Failed \n";
      exit(1);
    }
  }

  void send_and_rec() {
    signal(SIGINT, my_handler);
    
    cout << "Enter the command :\n ";
    // readline library
    while ((line = readline(">>> ")) )//!= nullptr)
    {
    	  if (*line)
		  add_history(line);
      int var = strlen(line);
      if(var>0)
      {
    	  send(sock_fd, line, var, 0);
      }
    else
    {
      //if input is null, it is a error
      string s="error";
      send(sock_fd,s.data(),s.length(),0);
    }
    
	  free(line);
      

      recv(sock_fd, buffer, sizeof(buffer), 0);
   //   cout<<"Received:\n";
      cout << buffer <<endl;
      if (strcmp(buffer, "CLIENT_SHUTDOWN") == 0 ||
          strcmp(buffer, "SHUTDOWN") == 0)

      {
        clear_history();
        exit(0);
      }
    }
    return;
  }

  static void my_handler(int sig) { cout << "Enter SHUTDOWN to exit\n"; }
};

int main() {
  client myclient;
  myclient.create_socket();
  myclient.connection();
  myclient.send_and_rec();
  return 1;
}
