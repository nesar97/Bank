-------------------------------------------------------------------------
                       Banking server
------------------------------------------------------------------------
This banking server is used to maintain the user account details.
Banking process allow multiple customers in parallel (concurrently).
Banking process allow customers to connect from remote hosts.


tar file has 2 folders, 1.source and 2. reports 

1.source has 8 files

1.fb1.l   ----------------------> lex file
2.fb1.y   ----------------------> bison file
3.struct.h ---------------------> global structure file
4.mydata.h / mydata.cpp --------> c++ file for data strutures
5.serverclass.h/ serverclass.cpp -->c++ file for server functions
6.main.cpp ---------------------> c++ file  main function file
7.clientclass.cpp --------------> c++ file for client use
8.README.txt -------------------> readme file
9.input.txt --------------------> input test case file
10.input2.txt ------------------> 2nd input file

2. Reports has 3 files

1.val_report  ------------------>valgrind memcheck report
2.callgrind.out.5932 ----------->call grind report
3.massif.out.6361 -------------->massif report


------------------------------------------------------------------------






---------------------------------------------------------------------------
                       PRE REQUISITES
----------------------------------------------------------------------------

1.readline library

	it can be installed by 

		sudo apt-get install lib64readline7 lib64readline6-dev

2.install Make 
	it can be installed by

		sudo apt-get install make
--------------------------------------------------------------------------




--------------------------------------------------------------------------
                       HOW TO COMPILE
--------------------------------------------------------------------------
1. To compile server

	make

2.to compile client

	make client
-------------------------------------------------------------------------









----------------------------------------------------------------------------
                          HOW TO RUN
----------------------------------------------------------------------------
1.To run server:

		./main

2.To run client:

		./client < <your_input_file>	


                ----------OR----------------
		
		./client

------------------------------------------------------------------------------






-----------------------------------------------------------------------------------------------------
                 COMMANDS SUPPORTED AND ITS SYNTAX
-----------------------------------------------------------------------------------------------------

1.OPEN ACCOUNT <CustomerName>                    -------opens account and generate account number
2.CLOSE ACCOUNT <AccountNumber>                  -------closes the account
3.SHOW ACCOUNT BALANCE <AccountNumber>           -------shows the account balance of the user
4.DEPOSIT <AccountNumber> <Amount in decimal>    -------used for deposit
5.WITHDRAW <AccountNumber> <Amount in decimal>   -------used for withdraw
6.SHOW MINI STATEMENT <AccountNumber>            -------last 10 transaction
7.SHOW ALL ACCOUNTS                              -------shows all the accounts in bank
8.SHOW BANK BALANCE                              -------shows total bank balance
9.SHOW ACTIVE CONNECTIONS                        -------show IP and Port number of all connected clients.
10.SHUTDOWN 	                                 -------shutdown server/client

---------------------------------------------------------------------------------------------------------








