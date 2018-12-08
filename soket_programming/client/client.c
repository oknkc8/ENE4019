//성창호 2017029807

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP "127.0.0.1"
#define MAXBUF 1024
#define INVALID_SOCKET 0xffff
#define SOCKET_ERROR -1

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef int SOCKET;

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("ERROR : Command parameter does not right.\n");
		exit(0);
	}

	SOCKET s;
	sockaddr_in addr;
	int file_fd, PORT = atoi(argv[2]);
	int file_name_len, read_len;
	char buf[MAXBUF];
	strcpy(buf, argv[1]);

	// socket()
	s = socket(PF_INET, SOCK_STREAM, 0);
	if(s == INVALID_SOCKET){
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);
	if(connect(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR){
		perror("connect : ");
		printf("ERROR : Fail to connect.\n");
		close(s);
		return 1;
	}

	file_name_len = strlen(buf);

	send(s, buf, file_name_len, 0);
	file_fd = open(buf, O_RDONLY);
	if(!file_fd){
		perror("error : ");
		return 1;
	}

	// send()
	while(1){
		memset(buf, 0, MAXBUF);
		read_len = read(file_fd, buf, MAXBUF);
		send(s, buf, read_len, 0);
		if(!read_len){
			break;
		}
	}

	return 0;
}