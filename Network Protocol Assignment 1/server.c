#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* Receive buffer */
char recvBuff[1500];
char response[1600];

int handleClient(int clientSocket)
{
    int msgSize;

    /* Clear the receive and response buffers */
    memset(recvBuff, 0, 1500);
    memset(response, 0, 1600);

    if((msgSize = recv(clientSocket, recvBuff, 1500, 0)) < 0)
    {
        printf("Failed to receive on client socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    printf("Received:\n%s\n", recvBuff);

    /* Prepare response message */
    sprintf(response, "Got it: %s", recvBuff);

    /* Send the response string on the socket to client */
    send(clientSocket, response, strlen(response), 0);

    /* Close the socket */
    close(clientSocket);

    return 0;
}

int setupTCPServer(unsigned int portNum, int maxConn)
{
    int servSocket;
    struct sockaddr_in servAddr;

    /* Setup address */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(0);    /* Accept connections on any IP address */
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if((servSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Bind socket for server */
    if(bind(servSocket,(struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    {
        printf("Failed to bind socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Setup socket for listening */
    if((listen(servSocket, maxConn)) < 0)
    {
        printf("Failed to listen on socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* return the socket handle */
    return servSocket;

}

int main(int argc, char *argv[])
{
    int mySocket, clientSocket;
    struct sockaddr_in clientAddr;
    unsigned int clientLen = sizeof(clientAddr);
    unsigned int portNum;

	if (argc < 2) 
	{
		printf("Usage: testServer servPortNum\n");
		return -1;
	}
		
	/* Setup TCP port number */
	portNum = atoi(argv[1]);

    /* Setup the server TCP Socket */
    if((mySocket = setupTCPServer(portNum, 5)) < 0)
    {
        return -1;
    }

    /* Accept client connections */
    for(;;)
    {
        if((clientSocket = accept(mySocket, (struct sockaddr *) &clientAddr, &clientLen)) < 0)
        {
            printf("Failed to accept connection from client; Error %d: %s\n", errno, strerror(errno));
            return -1;
        }

        handleClient(clientSocket);
    }

    return 0;
}