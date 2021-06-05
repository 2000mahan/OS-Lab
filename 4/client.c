#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


void *runner(void *serv_sock)
{
	//printf("thread creation was successful\n");
    int server = (int)(long)serv_sock;
    char buffer[1024];
    // client thread always ready to receive message
    while(1){
    	bzero(buffer, 1024);
    	int valread = read(server, buffer, 1024);
    	if(strstr(buffer, "$$$") != NULL) {
    		break;
		}
    	printf(" %s\n", buffer);
    	if (valread < 0) {
			perror("read");
			exit(-1);
		}
		
    }
    printf("\n~bye\n");
    pthread_exit(0);
}

int main(int argc, char const *argv[]) {

	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	char host_name [100];
	char client_name[100];


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	
	// sets all memory cells to zero
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	
	// sets port and address family
	strcpy(host_name, argv[1]);
	int PORT = atoi(argv[2]);
	strcpy(client_name, argv[3]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, host_name , &serv_addr.sin_addr) <= 0)
	{
		printf("\n Invalid address/ Address not supported \n");
		return -1;
	}
	
	
	// connects to the server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	printf(" [CLIENT] client %s connected to the server [CLIENT]\n", client_name);
	if(send(sock, client_name, strlen(client_name), 0)<0){
		perror("send failed");
		exit(-1);
	}
	// create a thread for listening to the server
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, runner, (void *)sock);

	// reading requests from commandline
	printf(" [CLIENT] now ready to read user requests [CLIENT]\n");

	char str[600] = {0};
	while(fgets(str,600,stdin) > 0) {
			char msg[600] = {0};
	        strcpy(msg,client_name);
	        strcat(msg,"$");
	        strcat(msg,str);
			int val = send(sock, msg, strlen(msg), 0);
			if (val < 0){
				perror("message not sent");
				return -1;
			}
			if(strstr(str, "quit ") != NULL) {
    			break;
			}
	}
	pthread_join(tid,NULL);
	close(sock);
	exit(0);
	return 0;
}
