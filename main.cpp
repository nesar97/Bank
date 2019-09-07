#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>
#include "serverclass.h"                                                  
#include "mydata.h"
#include "struct.h"
using namespace std;

extern void my_buf_func(char *buf);
extern void destroy();
extern int yyparse();
extern details val;
mydata list;  // list for keeping track of accounts
map<int, string> active_connections;
pthread_mutex_t my_mutex;
int old_socket_fd; 


//thread function starts here

void *myfunc(void *arg) {
  int newsock_fd = *((int *)arg);
  char buffer[1024] = {0};
  char buffer2[5000] = {0};
  
  string ob = "";
  /* creating local structure to copy values from global structure
   *   this structure is local to the thread                          */
  struct local_struct {
    int account_number;
    string account_name;
    float money;
    int option;
  };

  while (1) {
    bzero(buffer, 1024);
    recv(newsock_fd, buffer, 1024, 0);


    struct local_struct ls;

    /*************mutex lock**************************************************/
    pthread_mutex_lock(&my_mutex);
    my_buf_func(buffer);
    yyparse();
    destroy();
    // COPYING FROM GLOBAL TO LOCAL STRUCTURE
    ls.account_number = val.accno;
    ls.account_name = val.username;
    ls.option = val.option;
    ls.money = val.amount;
    pthread_mutex_unlock(&my_mutex);
    /**************mutex unlocked********************************************/

    // Execute function
    list.execute(ls.option, ls.account_name, ls.money, ls.account_number, ob,
                 newsock_fd);
    bzero(buffer, 1024);
    send(newsock_fd, ob.c_str(), ob.length() + 1, 0);
    // SHUTDOWN only client, not server
    if (ob == "CLIENT_SHUTDOWN")  
    {
      close(newsock_fd);
      pthread_exit(NULL);
      return NULL;
    }
    
    // SHUTDOWN SERVER TOO
    if (ob == "SHUTDOWN")  
    {
      close(newsock_fd);
      close(old_socket_fd);
      cout << "Server is shutting down";
      exit(0);
    }
    bzero(buffer2, 5000);  
    ob = "";
  }
}


/*
main function creates server object and creates thread */
int main() {
  int newsock_fd, ret;
  string s = "";
  server my_server; 
  old_socket_fd = my_server.sock_bind_listen();
  pthread_t tid[10]; 
  int i = 0;
  while (1) {
    newsock_fd = my_server.myaccept();
    // Initializing the mutex lock
    ret = pthread_mutex_init(&my_mutex, NULL);
    if (ret != 0) {
      perror("mutex init failed\n");
      exit(EXIT_FAILURE);
    }
    if (newsock_fd > -1) {
      //creating thread for each client
      if (pthread_create(&tid[i++], NULL, myfunc, &newsock_fd) !=
          0)  
        printf("Failed to create thread\n");
    } else {
      perror("accept");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
