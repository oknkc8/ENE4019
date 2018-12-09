# Socket Programming

### 2017029807 성창호



본 과제는 연결 - 지향 (Connection - oriented) 응용 프로토콜을 기반으로 하여 C를 이용해작성했으며, Linux 18.04 LTS 환경에서 코드를 작성하였다.



### `server.c`

------

`server.c`는 Socket Programming에서 server 부분을 구현해놓은 코드이다. 대략적인 흐름은 다음과 같다.

1. server socket 생성
2. client 접속 요청 확인
3. client socket 생성
4. client 접속 요청에 따라 접속 수락
5. client가 송신한 데이터 수신 & 수신 데이터 저장
6. server socket & client socket 소멸 후, 데이터 통신 종료

참고로 `server.c`의 실행 파일인 `server`는 인자로 client으로 부터 수신한 데이터를 저장하는 파일 이름과 port 번호를 받는다.



#### 1. server socket 생성

```c
// create sever socket
server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
if(server_sockfd == -1){
	perror("EROOR : Socket - ");
	exit(0);
}
```

본 과제는 TCP/IP에서 수행되므로 `socket`함수의 인자로 `SOCK_STREAM`을 넘겨준다. `socket`함수의 반환 값이 -1일 경우 socket이 생성되지 않은 것이므로 예외처리를 해준다.

```c
// bind
bzero(&serveraddr, sizeof(serveraddr));
serveraddr.sin_family = AF_INET;					// IPv4 Internet Protocol
serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);		// 32bit IPv4 Address
serveraddr.sin_port = htons(PORT);					// PORT Number

if(bind(server_sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) > 0){
	perror("ERROR : Bind - ");
	exit(0);
}
```

server socket에 필요한 정보들을 담는 `sockaddr_in` 구조체인 `serveraddr`를 초기화한다. 본 과제는 IPv4 인터넷 프로토콜을 이용하고,  32bit IPv4 주소를 이용하므로 위와 같이 `serveraddr`에 값을 할당한다. 그 후, `bind`함수를 이용하여 정보를 할당한 server socket을 커널에 등록한다. 커널에 server socket이 등록되야 다른 시스템과 통신할 수 있는 상태가 된다. 즉, 커널이 socket을 이용하여 외부로부터의 자료를 수신할 수 있게 된다.



#### 2. client 접속 요청 확인

```c
// listen
if(listen(server_sockfd, 5) != 0){
	perror("ERROR : Listen - ");
	exit(0);
}
```

`listen` 함수를 호출하면 client의 접속 요청이 올 때 까지 대기 상태가 된다. 만약 `listen` 함수가 ㅔERROR를 반환하지 않았다면, client의 접속 요청이라고 본다.



#### 3. client socket 생성 & 4. client 접속 요청에 따라 접속 수락

```c
// create client socket & accept connect request
client_sockfd = accept(server_sockfd, (sockaddr*)&clientaddr, &client_len);
```

`accept` 함수를 이용하여 접속 요청을 허락하게 되면 client와 통신하기 위해 커널이 자동으로 client socket을 생성한다.



#### 5. client가 송신한 데이터 수신 & 수신 데이터 저장

```c
// receive data & save data on file
file_read_len = recv(client_sockfd, buf, MAXBUF, 0);
write(file_fd, buf, file_read_len);
```

client socket이 생성되었으므로 `recv` 함수를 이용하여 `buf` 배열에 수신된 데이터를 저장한다. 그리고 `recv ` 함수는 수신된 데이터의 크기를 반환하므로 이를 `file_read_len`에 저장한다. 그리고 `write` 함수와 미리 정해놓은 파일 디스크립터인 `file_fd`를 이용하여 `buf`의 내용을 파일에 쓰고 저장한다.



#### 6. server socket & client socket 소멸 후, 데이터 통신 종료

```c
// close sockets
if(file_read_len == EOF || file_read_len == 0){
	printf("Receiving Success!\n");
	close(client_sockfd);
	close(file_fd);
	break;
}
close(server_sockfd);
```

만약 수신된 데이터가 더이상 없다면, client socket와 server socket을 `close` 함수를 이용하여 소멸시키고 파일 디스크립터도 소멸시킨 후, 데이터 통신을 종료한다.





### `client.c`

------

`client.c`는 Socket Programming에서 client 부분을 구현해놓은 코드이다. 대략적인 흐름은 다음과 같다.

1. socket 생성
2. server와 연결 시도
3. 생성된 socket으로 데이터 송신
4. socket 소멸 후, 데이터 통신 종료

참고로 `client.c`의 실행 파일인 `client`는 인자로 송신하는 데이터가 저장되어 있는 파일 이름과 port 번호를 받는다.



#### 1. socket 생성

```c
// socket create
s = socket(PF_INET, SOCK_STREAM, 0);
if(s == INVALID_SOCKET){
	return 1;
}
```

`server.c`에서 설명한 것과 같다.



#### 2. server와 연결 시도

```c
// try to connect to server
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr(IP);
addr.sin_port = htons(PORT);
if(connect(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR){
	perror("ERROR : connect - ");
	printf("ERROR : Fail to connect.\n");
	close(s);
	return 1;
}
```

`sockaddr_in` 구조체인 `addr`에 socket 정보를 할당하고 `connect` 함수를 이용하여 server와 연결을 시도한다. 만약 ERROR를 반환한다면, 1에서 생성된 socket을 소멸시키고 종료한다.



#### 3. 생성된 socket으로 데이터 통신

```c
// send data
while(1){
	memset(buf, 0, MAXBUF);
	read_len = read(file_fd, buf, MAXBUF);
	send(s, buf, read_len, 0);
	if(!read_len){
		printf("Sending Success!\n");
		break;
	}
	printf("%d bytes sended.\n", read_len);
}
```

`read` 함수를 이용하여 인자로 받은 파일로 부터 데이터를 읽어 `buf`에 저장한 후, `send` 함수를 이용하여 server에 송신한다. 만약 파일로 부터 읽어들인 데이터가 없다면 송신을 종료한다.



#### 4. socket 소멸 후, 데이터 통신 종료

`server.c`에서 설명한 것과 같다.





### 실행 결과

------

##### `send.txt`(송신 데이터)

![1544341256832](C:\Users\oknkc\AppData\Roaming\Typora\typora-user-images\1544341256832.png)



##### server 실행 화면

![1544341568391](C:\Users\oknkc\AppData\Roaming\Typora\typora-user-images\1544341568391.png)



##### client 실행 화면

![1544341592347](C:\Users\oknkc\AppData\Roaming\Typora\typora-user-images\1544341592347.png)



##### `rec.txt` (수신 데이터)

![1544341634437](C:\Users\oknkc\AppData\Roaming\Typora\typora-user-images\1544341634437.png)









































