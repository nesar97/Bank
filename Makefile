# to compile .l  .y  and server files

CC=g++
CFLAGS= -g -Wall 
SERFLAG= -ll -lpthread
READLINE= -lreadline 


#edit:  main.o serverclass.o mydata.o
#	$(CC) -o edit fb1.tab.c lex.yy.c main.o serverclass.o mydata.o 

main:lex.yy.c fb1.tab.c mydata.o serverclass.o main.cpp
	$(CC) fb1.tab.c lex.yy.c main.cpp serverclass.o mydata.o $(CFLAGS) ${SERFLAG} -o $@

serverclass.o: serverclass.h serverclass.cpp
	$(CC) serverclass.cpp $(CFLAGS) ${SERFLAG} -c 

mydata.o: mydata.cpp mydata.h
	$(CC)  mydata.cpp $(CFLAGS) ${SERFLAG} -c 

lex.yy.c:fb1.l
	flex fb1.l 

fb1.tab.c:fb1.y
	bison -dv fb1.y 

client: clientclass.cpp
	$(CC) clientclass.cpp $(READLINE) -o client
	


 

