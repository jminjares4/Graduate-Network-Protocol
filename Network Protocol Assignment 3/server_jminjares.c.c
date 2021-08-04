/*
	JESUS MINJARES
	EE 5379
	Programming Assignment #3
	April 18, 2020
*/ 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define OP_SUM 0
#define OP_MEAN 1
#define OP_MIN 2 
#define OP_MAX 3

void* handleClients(void *client);
double answer(double *data, int ptr[2]);//computation functions 
int main()
{
	int server_socket, client_socket, i; //established server and client socket 
	server_socket = socket(AF_INET, SOCK_STREAM, 0); //set socket for a TCP connection 
	
	//define the address of the server and set the address parameters 
	struct sockaddr_in server_address, clientAddr; //server and client address 
	server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(20000);
	server_address.sin_addr.s_addr = htons(0);//INADDR_ANY;
	
	unsigned int clientLen = sizeof(clientAddr); //get the length of the address accept 
	
	//bind the sock by passing server socket through bind();
	if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		//check for error when bind 
		printf("ERROR AT BINDING\n");
		exit(0);  
	}
	
	printf("BINDED\n");
	if(listen(server_socket, 5) < 0)
	{
		//check for error at listening 
		printf("ERROR AT Listening\n");
		exit(0);
	}
	printf("Listening...\n");
	
	while(1){
	//accept a client by passing the server_socket and its address 
	if(	(client_socket = accept(server_socket, (struct sockaddr*)&clientAddr, &clientLen)) < 0)
	{
		//check for error at accept a new client 
		printf("ERROR\n");
		return -1;
	}
	pthread_t t; //create a pthread variable 
	pthread_create(&t, NULL, &handleClients, &client_socket);//create a pthread and provide its start routine and pass variable 
	}

	return 0;
}
//start up routine for the threads
void* handleClients(void *client)
{
	int client_socket = *(int*)client; //dereference the void pointer into a int
	
	double number; //store the computation for the client 
	double dataBuffer[128];
	recv(client_socket, (void*)&dataBuffer, sizeof(dataBuffer), 0);
	
	int ptr[2]; //create an array to store the structure integer of the client
	memcpy(&ptr,&dataBuffer, sizeof(int)*2);

	//print out Clients info of the structure 
	printf("\n-----------Client Request-----------\n");
	printf("OPERATION: %i\n", ptr[0]); //prompt the operation 
	for(int i = 2; i < ptr[1] + 2; i++)
		printf("BUFFER[%i]-> %lf\n",i, dataBuffer[i]);// iterate over hte buffer 
	
	number = answer(dataBuffer, ptr); //call the function to compute the operation 
	
	printf("SERVER COMPUTATION: %lf\n", number);
	//send the computation answer to the client 
	send(client_socket, (void *)&number, sizeof(number), 0);
	
	//close the current client 
	close(client_socket); 
	
	pthread_exit(0); //exit the thread 

}
//pass reference the dataBuffer and the structure values save in the int pointer
double answer(double *data, int ptr[2])
{
    int i =2; //intitalize offset 
    double num = 0; //intialize num 
    switch(ptr[0]){ //enter operation desired 
    case OP_SUM: //test if 1
       for(; i < ptr[1] + 2; i++)
            num = num + data[i]; //add all the value into  num 
        break;
    case OP_MEAN: 
       for(; i < ptr[1] + 2; i++)
            num = num + data[i]; //add all the values into num 
        num = num/ptr[1]; // divide the by the SIZE to get the mean 
         break;
    case OP_MIN: 
        num = data[i]; //set the initial values as the minimum 
        for( ; i < ptr[1] + 2; i++)
            if(num > data[i]) //test if the current index is minimum 
                num = data[i]; //save the new min value 
        break;
    case OP_MAX:
          num = data[i]; //set intial value as the max 
        for(; i < ptr[1] + 2; i++)
            if(num < data[i]) //test if the current value is greater 
                num = data[i]; //save the new max value 
        break;
    default:
        puts("\nINPUT INVALID:\n"); //if the user input invalid values 
        num = -1;
    }
    return num; //return the computation 
}


