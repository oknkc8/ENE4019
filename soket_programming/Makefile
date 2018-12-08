.SUFFIXES : .c .o
CC = gcc
CFLAGS = -O2 -g

OBJS1 = /server/server.o
OBJS2 = /client/client.o
SRCS = $(OBJS1: .o = .c) $(OBJS2: .o = .c)

all : server client

server : $(OBJS1)
	$(CC) -o server $(OBJS1)

client : $(OBJS2)
	$(CC) -o client $(OBJS2)

clean : 
	rm $(OBJS1) $(OBJS2) /server/server /client/client
