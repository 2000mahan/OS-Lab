#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


#define MESSAGE_SIZE 1024
#define MAX_CLIENT_COUNT 100
#define MAX_GROUP_COUNT 50

int client_count = 0;
int group_count = 1;
int group[MAX_GROUP_COUNT][MAX_CLIENT_COUNT];

void *listentoClient (void *c);
int parseCommand (int socket, char * msg);
bool validateGroupID (int g, int socket, char * name);
bool joinGroup(int client, int g);
bool leaveGroup(int client, int g);
void sendtoAll(int client, char * group_id, char * name, char * msg);
void mysend(int socket, char * str, int len);

int main(int argc, char const *argv[]) {

	pthread_t tid;
	pthread_attr_t attr; 
	pthread_attr_init(&attr);
	
	char buffer[100] = {0};
	

	// creates socket file descriptor 
	int server_fd;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// setting up the address variable
	int PORT = atoi(argv[1]);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
	const int addrlen = sizeof(address);
	
	
	// binding (bind the created socket to the address variable we set)
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	
	// listening on server socket with backlog size 3.
	if (listen(server_fd, 3) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	printf(" [SERVER] Listening on %s:%d [SERVER]\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	

	// initialize group array
	for (int i = 0; i < MAX_GROUP_COUNT; ++i)
		for (int j = 0; j < MAX_CLIENT_COUNT; ++j)
			group[i][j] = -1;

	// accepting client
	// accept returns client socket and fills given address and addrlen with client address information
	// client_socket is an identifier in order to conncet to the client socket
	int client_socket;
	while(1){
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		// print initial connection information
		printf(" [SERVER] connection established for client %s:%d [SERVER]\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		char * greetings = "Server : Hello Client!\n";
		send(client_socket, greetings , strlen(greetings), 0);

		//setting up a thread for each client
		client_count++;
		bzero(buffer, 100);
		if(read(client_socket, buffer, 100) < 0){
				perror("reading failed");
				exit(EXIT_FAILURE);
			}
		printf("\tclient name is: %s\n", buffer);
		if (pthread_create(&tid, &attr, listentoClient, (void *)client_socket)){
			perror("Thread creation failed");
            exit(EXIT_FAILURE);
		}
		
	}

	return 0;
}

void *listentoClient (void *c){

	int socket = (int)(long) c;

	printf(" [SERVER] waiting for user on socket %d [SERVER]\n", socket);

	// waits for client to send sth
	char buffer[MESSAGE_SIZE] = {0};
	int valread = -1;
	while(1){
        bzero(buffer, MESSAGE_SIZE);
        valread = read(socket , buffer, MESSAGE_SIZE); 
        if(valread >= 0){
            int ret = parseCommand(socket, buffer);
            if (ret == -1){
            	break;
            }
            if (ret == 1){
            	char str[20] = " [SERVER] : done ";
            	if(send(socket, str , strlen(str), 0)<0){
				perror("unable to send");
				exit(-1);
			}
        }
    }
        else{
        	perror("reading failed");
			exit(EXIT_FAILURE);
		}
	}
	if(send(socket, "$$$" , strlen("$$$"), 0)<0){
		perror("unable to send");
		exit(-1);
	}
	pthread_t pt = pthread_self();
	pthread_cancel(pt);
}

int parseCommand (int client, char * msg){
	
	char * name;
	char * gp_id;
	char * command;

	name = strtok(msg, "$");
	command = strtok(NULL, " ");

	//join request
	if (!strcmp(command, "join")){

		gp_id = strtok(NULL,"\0 ");
		printf("...join request by %s for group %s", name, gp_id);
		if (!validateGroupID(atoi(gp_id), client, name))
			return 0;
		if(!joinGroup(client, atoi(gp_id)))
			return 0;
	}
	// send message request
	else if (!strcmp(command, "send")){

		gp_id = strtok(NULL," ");
		char * pm;
		pm = strtok(NULL,"\n");
		printf("....send message request by %s to group %s", name, gp_id);
		if (!validateGroupID(atoi(gp_id), client, name))
			return 0;
		sendtoAll(client, gp_id, name, pm);
		return 0;
	}
	// leave group request
	else if (!strcmp(command, "leave")){

		gp_id = strtok(NULL,"\0 ");
		printf("...leaving request by %s for group %s", name, gp_id);
		if (!validateGroupID(atoi(gp_id), client, name)){
			return 0;
		}
		if(!leaveGroup(client, atoi(gp_id)))
			return 0;
	}
	// quit chatroom
	else if (!strcmp(command, "quit")){
		printf("...quit request by %s\n", name);
		for (int i = 0; i < MAX_GROUP_COUNT; ++i){
			for (int j = 0; j < MAX_CLIENT_COUNT ; ++j){
				if (group[i][j] == client){
					char gid[10] = {0};
					sprintf(gid, "%d", i+1);
					sendtoAll(client, gid, name, "left the chat!\n");
					group[i][j] = -1;
					break;
				}
			}
		}
		printf("done\n");
		return -1;
	}
	// wrong input
	else{
		char * str = "[SERVER] : invalid command, please try again";
		mysend(client, str, strlen(str));
		printf(" [SERVER] invalid command, CLIENT: %s must try again [SERVER]\n", name);
		return 0;
	}
	return 1;
}

bool validateGroupID(int g, int socket, char * name){

	if (g > MAX_GROUP_COUNT || g <= 0){
		printf("invalid group id by CLIENT: %s! try again between 1 and 50\n", name);
		char msg[600] = {0};
	    strcpy(msg,"[SERVER] : invalid group id, enter an integer between 1 and 50");
		mysend(socket, msg, strlen(msg));
		return false;
	}

	return true;
}

bool joinGroup(int client, int g){

	// search group for the client
	for (int i = 0; i < MAX_CLIENT_COUNT; ++i){
		if (group[g-1][i] == client){
			printf("user is already a member of this group\n");
			char msg[100] = "[SERVER] : you are already a member of this group";
			mysend(client, msg, strlen(msg));
			return false;
		}
		if ( group[g-1][i] == -1){
			printf("user added to the group\n");
			group[g-1][i] = client;
			return true;
		}
	}
	printf("group is full\n");
	char msg[100] = "[SERVER] : sorry, this group is full";
	mysend(client, msg, strlen(msg));

	return false;
}

bool leaveGroup(int client, int g){
	
	int sum = 0;
	for (int i = 0; i < MAX_CLIENT_COUNT; ++i){
		if (group[g-1][i] == client){
			group[g-1][i] = -1;
			printf("user removed successfully\n");
			return true;
		}
		sum += group[g-1][i];
	}

	// user was nor a member of the group
	char msg[100] = {0};
	if( sum == -50 )
		strcpy(msg, "group is empty");
	else
		strcpy(msg, "not a memeber of this group");
	char str[100]= " [SERVER] : ";
	strcat(str, msg);
	mysend(client, str, strlen(str));
	
	return false;
}


void sendtoAll(int client, char * group_id, char * name, char * msg){

	int g = atoi(group_id);

	// check membership
	bool memeber = false;
	for (int i = 0; i < MAX_CLIENT_COUNT; ++i){
		if (group[g-1][i] == client){
			memeber = true;
		}
	}
	if (memeber == false){
		printf("not a memeber of this group\n");
		char msg[100] = "[SERVER] : you are not a member of this group";
		mysend(client, msg, strlen(msg));
		return;
	}

	// build message
	char str1[MESSAGE_SIZE] = {0};
	strcpy(str1," Group ");
	strcat(str1, group_id);
	strcat(str1, " | ");
	char str2[MESSAGE_SIZE] = {0};
	strcpy(str2," [ ");
	strcat(str2, name);
	strcat(str2, " ] : ");
	strcat(str2, msg);

	// send message to all members of the group
	for (int i = 0; i < MAX_CLIENT_COUNT; ++i){
		if (group[g-1][i] != -1){
			int socket = group[g-1][i];
			mysend(socket, str1, strlen(str1));
			mysend(socket, str2, strlen(str2));
		}
	}
	printf("\nmassage sent to all\n");
	return;
}

void mysend(int socket, char * str, int len){
	char string[len+10];
	strcpy(string, str);
	if(send(socket, string , strlen(string), 0)<0){
		perror("send error");
		exit(-1);
	}
}

 