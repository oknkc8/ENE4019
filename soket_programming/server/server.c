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

#define MAXBUF 1024

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("ERROR : Command parameter does not right.\n");
		exit(0);
	}

	int server_sockfd, client_sockfd, PORT = atoi(argv[1]);
	int file_fd;	// sending file's file descriptor
	sockaddr_in serveraddr, clientaddr;
	int client_len, read_len, file_read_len;
	char buf[MAXBUF];

	client_len = sizeof(clientaddr);

	// socket()
	server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1){
		perror("socket error : ");
		exit(0);
	}

	// bind()
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	if(bind(server_sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) > 0){
		perror("bind error : ");
		exit(0);
	}

	//listen
	if(listen(server_sockfd, 5) != 0){
		perror("listen error : ");
		exit(0);
	}

	while(1){
		char file_name[MAXBUF];
		memset(buf, 0, MAXBUF);

		// accept
		client_sockfd = accept(server_sockfd, (sockaddr*)&clientaddr, &client_len);
		printf("Client Connect!\nIP Address : %s	PORT : %d\n\n", inet_ntoa(clientaddr.sin_addr), PORT);

		// file name
		read_len = read(client_sockfd, buf, MAXBUF);
		if(read_len <= 0){
			close(client_sockfd);
			break;
		}
		strcpy(file_name, buf);
		printf("%s > %s\n", inet_ntoa(clientaddr.sin_addr), file_name);

		// create file
		file_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
		if(!file_fd){
			perror("file open error : ");
			break;
		}

		// file save
		while(1){
			memset(buf, 0, MAXBUF);
			file_read_len = read(client_sockfd, buf, MAXBUF);
			write(file_fd, buf, file_read_len);

			if(file_read_len == EOF || file_read_len == 0){
				printf("finsh file\n");
				break;
			}
		}

		close(client_sockfd);
		close(file_fd);
	}

	close(server_sockfd);

	return 0;
}