#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>
#include<errno.h>

#define SOCKETNUMBER 3000
#define REQBUFSIZE 5000
#define RESBUFSIZE 500
#define MAXPENCON 10
#define RESDEFBUFSIZE 50
#define HIGHESTSOCKETNUM 20

char buf[REQBUFSIZE]={0};
char res[RESBUFSIZE];
char resDef[RESDEFBUFSIZE]="HTTP/1.1 404\n\nDocument not found";

int checkReq(char *buf){

	if( strncmp(buf+4, "/ ", 2) == 0 || strncmp(buf+4, "/index.html", 2) == 0)return 1;
	if(strncmp(buf+4, "/favicon.ico", 12) == 0) return 2;		
	return 0;
}

int handleconnection(int socket){
	printf("\nConnection astablished\n");		
	if(recv(socket,buf,REQBUFSIZE, MSG_DONTWAIT) == -1){
		if(errno == EAGAIN || errno == EWOULDBLOCK) return 0; 
		perror("read");
		exit(EXIT_FAILURE);	
	};
	printf("\nBuffer content=%s\n",buf);

	switch(checkReq(buf)){
		case 1:	
			write(socket, res, strlen(res));
			break;
		case 2:
			write(socket, res, strlen(res));
			break;
		default:
	       		write(socket, resDef, RESDEFBUFSIZE);
			break;
	}

	for(int i=0; i<REQBUFSIZE; i++){
		buf[i]=0;
	}
return 0;}


int main(){
	
	strcpy( res, "HTTP/1.1 200 OK\nConnection: close\n\n<!DOCTYPE html> <html lang=\"en-US\"><head><link rel=\"icon\" href= \"data:;base64,=\"></head><body>Hello world!</body></html>");

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
		printf("\n\n\n-------------------------\nWaiting for connections\n-------------------------\n");
		con=accept(sfd,NULL,NULL);
		handleconnection(con);
		close(con);
	}
	return 0;}
