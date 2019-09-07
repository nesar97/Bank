#include <stdlib.h>
#include <iostream>
#include "mydata.h"
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <map>
#include "serverclass.h"
#define HOWMANY 9
extern map <int,string> active_connections;
extern int old_socket_fd;
using namespace std;

//constructor
mydata::mydata()
{
	this->head = NULL;
	pthread_rwlock_init(&my_lock, NULL);
	i=1;
    
}

//destructor
mydata::~mydata()
{
	Node *temp = this->head;
	
        if(temp==NULL)
        {
        return ;
        }
	Node *curr=this->head;
	Node *next;
        while(curr)
        {
	next=curr->link;
        delete(curr);
	curr=next;
        
        }
        pthread_rwlock_destroy(&my_lock);
}

/* account number can be either random or incremental value 
   for simplicity , random is not used
*/
int mydata::open_account(string name,string &ob)
{
	//	srand(time(0));
	//	time_t t;
		pthread_rwlock_wrlock(&my_lock);
		//srand(i++);
		Node  *temp = new Node();
		temp->acc_no =i++;  //rand()%1000000;
		temp->balance=0;
		temp->acc_name=name;    
		temp->link =this-> head;   
		this-> head = temp;           
		ob+="\nAccount created and your account number is: "+ to_string(temp->acc_no)+"\n";
		pthread_rwlock_unlock(&my_lock);
		return 1;
}


// traverse the list and print name and account number
int mydata::show_all_accounts(string &ob)
{
	pthread_rwlock_rdlock(&my_lock);
	Node *temp = this->head;
	if(temp==NULL)
	{
	ob+="No accounts\n";
	pthread_rwlock_unlock(&my_lock);
	return 1;
	}
	
	while(temp)
	{
        ob+="\nAccount name : "+ temp->acc_name + " Account number : "+ to_string(temp->acc_no);
        temp = temp->link;	
       	}
	pthread_rwlock_unlock(&my_lock);
    	return 1;
}


int mydata::close_account(int acc_num,string &ob )
{
	pthread_rwlock_wrlock(&my_lock);
	Node *prev=this->head;

	if(prev==NULL)
	{
		ob+="No accounts to delete\n";
		pthread_rwlock_unlock(&my_lock);
		return 1;
	}
	if( acc_num==prev->acc_no)
	{
	this->head=prev->link;
	delete (prev);
	ob+="\nAccount "+to_string(acc_num)+" deleted\n";
	pthread_rwlock_unlock(&my_lock);
	return 1;
	}
	Node *curr=this->head->link;

		
	while(curr!=NULL && curr->acc_no!=acc_num)
	{
		prev=prev->link;
		curr=curr->link;
	}

	if(curr==NULL)
	{
		ob+="Account not found\n";
		pthread_rwlock_unlock(&my_lock);
		return 0;
	}

	else 
	{
		prev->link=curr->link;
		delete (curr);
		ob+="\nAccount "+ to_string(acc_num)+" deleted\n";
		pthread_rwlock_unlock(&my_lock);
		return 1;
	}
	
		
}

int mydata::deposit(int num,float money,string &ob)
{
	// error if money is negative value
	if(money<0)
	{
		ob+="Negative Error\n";
		return 0;
	}
		pthread_rwlock_wrlock(&my_lock);	
		Node *temp = this->head;

	if(temp==NULL)
	{
		ob+="No accounts created\n";
		pthread_rwlock_unlock(&my_lock);
		return 0;
	}

	while(temp!=NULL && temp->acc_no != num )
	{
		temp=temp->link;
	}
	
	if(temp==NULL)
	{
		ob+="Wrong account number\n";
		pthread_rwlock_unlock(&my_lock);
		return 0;
	}
	temp->balance+=money; 
	if(temp->myqueue_data.size()>9 || temp->myqueue_stats.size()>9)
	{
		temp->myqueue_data.pop();
		temp->myqueue_stats.pop();
	}
	temp->myqueue_data.push(money);
	temp->myqueue_stats.push("DEPOSIT");
	ob+="Deposited\n";
	pthread_rwlock_unlock(&my_lock);
	return 1;
	
}

// find the account and display balance
int mydata::show_account_balance(int num,string &ob)
{
	pthread_rwlock_rdlock(&my_lock);
	Node *temp = this->head;

        if(temp==NULL)
        {
		ob+="No accounts created\n";
		pthread_rwlock_unlock(&my_lock);
        return 0;
        }

        while(temp!=NULL && temp->acc_no != num)
        {
                temp=temp->link;
        }

        if(temp==NULL)
        {
		ob+="Wrong account number\n";
		pthread_rwlock_unlock(&my_lock);
		return 0;
        }
        ob+="The account balance is : "+to_string(temp->balance)+"\n";
		pthread_rwlock_unlock(&my_lock);
        return 1;
}

int mydata::withdraw(int num,float money,string &ob)
{
	// error if money is negative
 if(money<0)
        {
        ob+="Negative error\n";
        return 0;
        }
	pthread_rwlock_wrlock(&my_lock);
        Node *temp = this->head;

        if(temp==NULL)
        {
        ob+="No accounts created\n";
	pthread_rwlock_unlock(&my_lock);
        return 0;
        }

        while(temp!=NULL && temp->acc_no != num)
        {
                temp=temp->link;
        }

        if(temp==NULL)
        {
        ob+="Wrong account number\n";
	pthread_rwlock_unlock(&my_lock);
        return 0;
        }
        
	// withdrawing more amount than that is present in balance is error	
	float *negative= new float;
	*negative=temp->balance-money;
	if(*negative<0)
	{
	ob+="Cannot withdraw \n";
	delete(negative);
	pthread_rwlock_unlock(&my_lock);
	return 0;
	}

	temp->balance -= money;
        if(temp->myqueue_data.size()>9 || temp->myqueue_stats.size()>9 )
	{
	temp->myqueue_data.pop();
        temp->myqueue_stats.pop();
	}
	temp->myqueue_data.push(money);
        temp->myqueue_stats.push("WITHDRAW");
	
	delete(negative);
	ob+="withdrawn";
	pthread_rwlock_unlock(&my_lock);
        return 1;


}



int mydata::show_bank_balance(string &ob)
{
	pthread_rwlock_rdlock(&my_lock);
	Node *temp = this->head;
        if(temp==NULL)
        {
        ob+="\nNo accounts\n bank balance : 0\n";
	pthread_rwlock_unlock(&my_lock);
        return 1;
        }
	
	float *total=new float;
	*total=0.0;
        while(temp)
        {
        (*total) += temp->balance;
        temp = temp->link;
        }

	ob+="The Bank balance is : "+to_string(*total);
	delete(total);
	pthread_rwlock_unlock(&my_lock);
        return 1;
	
}

/*display last 10 transactions.
 maintains all data in queue and pops out the old data */
int mydata::show_mini_statement(int num,string &ob)
{
	pthread_rwlock_rdlock(&my_lock);
        Node *temp = this->head;

        if(temp==NULL)
        {
        ob+="No accounts created\n";
	pthread_rwlock_unlock(&my_lock);
        return 0;
        }

        while(temp!=NULL && temp->acc_no != num)
        {
                temp=temp->link;
        }

        if(temp==NULL)
        {
        ob+="Wrong account number\n";
	pthread_rwlock_unlock(&my_lock);
        return 0;
        }
        ob+="The Mini statement is  :\n ";
	queue <float> copy1 = temp->myqueue_data;
	queue <string> copy2= temp->myqueue_stats;
	while (!copy1.empty() )
	{
        ob+= copy2.front();
        ob+= "  "+ to_string(copy1.front()) + "\n";
        copy1.pop();
	copy2.pop();
	}
	ob+="Balance available at end: "+ to_string(temp->balance)+"\n";	
	pthread_rwlock_unlock(&my_lock);
        return 1;
}


//iterates through map
int mydata::show_active_connections(string &ob)
{
	
  
        pthread_rwlock_rdlock(&my_lock);
	ob+="The active connections are:\n";

	
	map<int, string>::iterator itr;
	for (itr = active_connections.begin(); itr != active_connections.end(); ++itr)
	{
        	 ob+= itr->second + '\n';
    	}
        pthread_rwlock_unlock(&my_lock);
    	return 1;
}



int mydata::shutdown(string &ob,int fd)
{
	//last client ?	
	if(active_connections.size()==1)
	{
		ob+="SHUTDOWN";
		active_connections.erase(fd);
		return 1;
	}	
	else
	{
		//only shutdown client, not server
		ob+="CLIENT_SHUTDOWN";
		active_connections.erase(fd);
		return 1;

	}
}		


int mydata::mysyntax_error(string &ob)
{
	 ob+="SYNTAX ERROR \n";
	 return 1;
}


//function which calls all other functions according to the options
int mydata::execute(int option, string account_name, float money,int account_number,string &ob,int fd)
{


	 switch(option)
        {
                case 1:
		    	open_account(account_name,ob);
			break;

                case 2: 
			close_account(account_number,ob);
                        break;

                case 3: 
		       	show_account_balance(account_number,ob);
                        break;

                case 4: 
			deposit(account_number,money,ob); 
                        break;

                case 5: 
			withdraw(account_number, money,ob);
                        break;

                case 6: 
			shutdown(ob,fd);
                        break;


                case 7: 
			show_mini_statement( account_number,ob);
                        break;

                case 8: show_all_accounts(ob); break;

                case 9: show_bank_balance(ob); break;

                case 10: show_active_connections(ob); break;

                case -1: mysyntax_error(ob); break;

                default : mysyntax_error(ob);
			 
        }
	
	return 1;
}

