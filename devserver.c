#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>

#define SOCKETNUMBER 8080
#define REQBUFSIZE 500
#define RESBUFSIZE 60
#define MAXPENCON 10

int handleconnection(int socket){
	printf("\nConnection astablished\n");		
	
	char buf[REQBUFSIZE]= {0};
	
	read(socket,buf,REQBUFSIZE);
	
	char res[RESBUFSIZE]="HTTP/1 200\n\nHello world!";
	write(socket, res,RESBUFSIZE);
return 0;}


int main(){

	struct sockaddr_in addr={
		AF_INET,htons(SOCKETNUMBER),htonl(INADDR_LOOPBACK)
	};

	int sfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sfd == -1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if(bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if(listen(sfd,MAXPENCON) == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	int con;
	for(;;){
		printf("\n-------------------------\nWaiting for connections\n-------------------------\n");
		con=accept(sfd,NULL,NULL);
		handleconnection(con);
		close(con);
	}
		

return 0;}
