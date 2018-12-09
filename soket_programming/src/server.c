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
	if(argc != 3){
		printf("ERROR : Command parameter does not right.\n");
		exit(0);
	}

	int server_sockfd, client_sockfd, PORT = atoi(argv[2]);
	int file_fd;	// sending file's file descriptor
	sockaddr_in serveraddr, clientaddr;
	int client_len, read_len, file_read_len;
	char buf[MAXBUF], file_name[MAXBUF];
	int readBytes;
	strcpy(file_name, argv[1]);

	client_len = sizeof(clientaddr);

	// socket()
	server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1){
		perror("EROOR : Socket - ");
		exit(0);
	}

	// bind()
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	if(bind(server_sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) > 0){
		perror("ERROR : Bind - ");
		exit(0);
	}

	//listen
	if(listen(server_sockfd, 5) != 0){
		perror("ERROR : Listen - ");
		exit(0);
	}

	int flag = 0;
	while(1){
		memset(buf, 0, MAXBUF);
		printf("Listening...\n");

		// accept
		client_sockfd = accept(server_sockfd, (sockaddr*)&clientaddr, &client_len);
		printf("Client Connect!\nIP Address : %s	PORT : %d\n", inet_ntoa(clientaddr.sin_addr), PORT);

		// create file
		file_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
		//readBytes = recv(client_sockfd, buf, )
		if(!file_fd){
			perror("ERROR : File open - ");
			break;
		}

		// file save
		while(1){
			memset(buf, 0, MAXBUF);
			file_read_len = recv(client_sockfd, buf, MAXBUF, 0);

			if(file_read_len == EOF || file_read_len == 0){
				printf("Receiving Success!\n");
				close(client_sockfd);
				close(file_fd);
				flag = 1;
				break;
			}
			printf("%d bytes received.\n", file_read_len);
			write(file_fd, buf, file_read_len);
		}

		if(flag) break;

		close(client_sockfd);
		close(file_fd);
	}

	close(server_sockfd);

	return 0;
}