#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void client_file(int socket_desc, struct sockaddr_in addr)
{
	FILE *file1;
	char *file_name = "clientFile.txt";
	int a;
	char buffer[4000];
	socklen_t addr_size;

	file1 = fopen(file_name, "w");

	while(1)
	{
	 a =recvfrom(socket_desc, buffer, 4000, 0, (struct sockaddr*)&addr, &addr_size);

	if (strcmp(buffer, "END") == 0)
	{
	 break;
	 return;
	}

	printf("[RECIEVE] File Data: %s", buffer);
	fprintf(file1, "%s", buffer);
	bzero(buffer, 4000);
	}

	fclose(file1);
	return;
}


int main(int argc, char *argv[])
{
	int socket_desc;
	struct sockaddr_in server = {0};
	struct sockaddr_in client;
	char creply[4000] = {0};
	
	//create socket
	socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_desc == -1)
	{
	 printf("Could not create socket");
	}
	printf("[+] Socket created successfully!\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	
	//bind
	
	if( bind(socket_desc,(const struct sockaddr *)&server ,sizeof(server)) < 0)
	{
	 puts("bind failed");
	 return 1;
	}
	puts("[+] Bind successfull!  \n");

	//listen
	listen(socket_desc,3);
	printf("[+] Waiting for incoming connections... \n");

 
	 //message from client
	socklen_t len = 0;
	int n = recvfrom(socket_desc, (char *)creply, 4000, MSG_WAITALL, 0, &len);

        creply[n] = '\n';
	printf("%s",creply);

	printf("\n[STARTING] TFTP File Server started. \n");
	client_file(socket_desc, client);

	printf("\n[SUCCESS] File Transfer Complete!.\n");
	printf("[CLOSE] Shutdown the tftp server...\n");

	close(socket_desc);
	return 0;

}
