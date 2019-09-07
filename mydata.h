#ifndef MYDATA_H
#define MYDATA_H
#include<string>
#include<queue>
using namespace std;
class Node
{
	public:
		Node *link;         
		int acc_no;
		float balance;
		string acc_name;
		queue <float> myqueue_data;     // queue for maintaining transactions
		queue <string> myqueue_stats; 

};

class mydata
{
	public:
		class Node *head;
   		pthread_rwlock_t my_lock;	      	
		int i;
		mydata();
		~mydata();
		int open_account(string name,string &ob);
		int close_account(int num,string &ob);
		int show_account_balance(int num,string &ob);
     	int deposit(int num,float money,string &ob);
		int withdraw(int num,float money,string &ob);
		int show_mini_statement(int num,string &ob);
		int show_all_accounts(string &ob);
		int show_bank_balance(string &ob);
		int show_active_connections(string &ob); 
		int shutdown(string &ob,int fd);   
		int mysyntax_error(string &ob);
		int execute(int option, string account_name, float money,int account_number,string & ob,int fd);
};


#endif
