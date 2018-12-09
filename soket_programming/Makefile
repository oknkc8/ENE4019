# Compiler
CC = gcc

# Macros specifying path for compile
SRC = ./src/
SERVER = ./bin/server/
CLIENT = ./bin/client/

# Compile server.c
server: $(SRC)server.c
	$(CC) -o $(SERVER)server $(SRC)server.c

# Compile client.c
client: $(SRC)client.c
	$(CC) -o $(CLIENT)client $(SRC)client.c

all: server client

# Clean
clean:
	rm $(SERVER)server
	rm $(CLIENT)client
