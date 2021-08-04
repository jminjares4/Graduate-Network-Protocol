/*
	JESUS MINJARES
	EE 5379
	Programming Assignment #2
	April 12, 2020
*/
#include <stdio.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#define OP_SUM 0
#define OP_MEAN 1
#define OP_MIN 2
#define OP_MAX 3
#define SIZE 10

typedef struct
{
 int operation; //contain the operation number 
 int size; //will be assign the total length of the computed numbers 
}sCOMP_REQ;

//will take a arguments and will return an integer if an error occurs 
int main(int argc, const char * argv[])
{
	if(argc < 3) //if user enter less or more arguments 
	{
		printf("./client operation size\n");
		return -1;
	}
	sCOMP_REQ request; //create the object of the structure sCOMP_REQ
	request.operation = atoi(argv[1]); //convert the string to integer for the operation 
	request.size = atoi(argv[2]); //convert the argument to integer for the size 
	
	double buffer[request.size + 2]; //create a buffer with an offset of 2
	memset(&buffer, 0, sizeof(buffer)); //set the all the buffer at 0
	
	memcpy(&buffer, &request, sizeof(request)); //copy memory of the structure into the buffer 
	
	double value; //value with store the server computation
	
	//create a socket and iterator  
	int mySocket, i; 
	mySocket = socket(AF_INET, SOCK_STREAM, 0); //established my socket configuration 
	
	//specify the address for the socket
	struct sockaddr_in server_address; //create server address 
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(20000); //load port 20000 
	server_address.sin_addr.s_addr = INADDR_ANY; // htons(0) //inet_addr("172.20.36.26");
	
	//connect the socket and pass the socket through connect() 
	int connection = connect(mySocket, (struct sockaddr *)&server_address, sizeof(server_address));
	
	if(connection < 0)  
		printf("ERROR\n\n");
	
	//gather the numbers to compute, offset the buffer by 2 to start
	//at the beginning of the list 
	for(int i = 2; i < request.size + 2; i++)
	{
		printf("Please enter number %d : ", i - 1); //prompt the user to enter the number 
		scanf("%lf",&buffer[i]); //save the number into the buffer 
	}
	
	//send the structure through the socket 
	send(mySocket, (void *)&buffer, sizeof(buffer),0);
	
	//receive the server computation stored in value
	recv(mySocket, (void *)&value, sizeof(value), 0);
	
	printf("\nSERVER FEEDBACK: %lf\n", value); 
	//close the socket
	close(mySocket);
	
	return 0; 
}
