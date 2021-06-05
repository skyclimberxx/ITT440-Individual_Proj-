#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void send_file(FILE *fp, int socket_desc,struct sockaddr_in addr)
{
	int n;
	char buffer[4000];

	while(fgets(buffer, 4000, fp) != NULL)
	{
	 printf("[SENDING] Data: %s", buffer);

	//send file to server
	n = sendto(socket_desc, buffer, 4000, 0, (struct sockaddr*)&addr, sizeof(addr));

	if( n == -1)
	{
	 printf("Failed sending file to the server...");
	}

	bzero(buffer, 4000);
	}

	//server close the file
	strcpy(buffer, "END");
	sendto(socket_desc, buffer, 4000, 0, (struct sockaddr*)&addr, sizeof(addr));

	fclose(fp);
	return;
}


int main(int argc, char*argv[])
{

	int socket_desc;
	struct sockaddr_in server = {0};
	char *message;
	FILE *fp;
	char *filename = "clientFile.txt";
	

	//create socket
	socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_desc == -1)
	{
	 printf("Could Not create a socket!");
	}
	printf("[+] Socket created successfuly!\n");

	server.sin_addr.s_addr = inet_addr("192.168.114.6");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Send some data 
	message = "Hi Server tftp , im from Facebook client..XOXO";
	int len = sendto(socket_desc, message, strlen(message),0,(const struct sockaddr *)&server, sizeof(server)); 
	if ( len == -1)
	{
	 puts("Message failed to send!");
	 return 1;
	}
	puts("[+] Data send Successfully! \n");

	//read & send file to server
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
	 printf("Could read the file!");
	}

	send_file(fp, socket_desc, server);

	printf("\n[SUCCESS] File transfer complete.\n");
	printf("[CLOSED] Disconnecting from the server...\n");

	close(socket_desc);
	return 0;
}
