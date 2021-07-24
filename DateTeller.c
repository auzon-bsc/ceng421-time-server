/*
CHAPTER 1

ifconfig
whois google.com
host google.com
nslookup google.com

CHAPTER 2

Page 43 || 59

#include <sys/types.h>
#include <sys/socket.h>

AF_INET / PF_INET - IPv4

SOCK_STREAM - TCP
SOCK_DGRAM - UDP
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include<time.h>

int main(int argc, char *argv[])
{
    time_t t;
    char* out;
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;
    /* make sure we have a port number */
    if (2 != argc)
    {
    	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    	exit(1);
    }

    /* CREATING SOCKET */
    /* create a streaming socket */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(simpleSocket == -1)
    {
    	fprintf(stderr, "Could not create a socket!\n");
    	exit(1);
    }
    else
    {
    	fprintf(stderr, "Socket created!\n");
    }

    /* BINDING */
    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);
    /* set up the address structure */
    /* use INADDR_ANY to bind to all local addresses */
    /* note use of htonl() and htons() */
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);

    simpleServer.sin_port = htons(simplePort);
    /* bind to the address and port with our socket */
    returnStatus = bind(simpleSocket, (struct sockaddr *)&simpleServer,
    sizeof(simpleServer));
    if (returnStatus == 0)
    {
      fprintf(stderr, "Bind completed!\n");
    }
    else
    {
      fprintf(stderr, "Could not bind to address!\n");
      close(simpleSocket);
      exit(1);
    }

    /* LISTEN */
    /* tell the socket we are ready to accept connections */
    returnStatus = listen(simpleSocket, 5);
    if (returnStatus == -1)
    {
      fprintf(stderr, "Cannot listen on socket!\n");
      close(simpleSocket);
      exit(1);
    }

    /* ACCEPT */
    while (1)
    {
      /* set up variables to handle client connections */
      struct sockaddr_in clientName = { 0 };
      int simpleChildSocket = 0;
      int clientNameLength = sizeof(clientName);
      /* block on accept function call */
      simpleChildSocket = accept(simpleSocket,
        (struct sockaddr *)&clientName, &clientNameLength);

      if (simpleChildSocket == -1)
      {
        fprintf(stderr, "Cannot accept connections!\n");
        close(simpleSocket);
        exit(1);
      }

      /* WRITING TO CLIENT */
      /* handle the new connection request */
      /* write out our message to the client */
      out = "connected...\n";
      write(simpleChildSocket, out, strlen(out));

      time(&t);
      out = ctime(&t);
      write(simpleChildSocket, out, strlen(out));

      out = "...disconnected..\n";
      write(simpleChildSocket, out, strlen(out));
      close(simpleChildSocket);
    }

    /* CLOSING SERVER SOCKET */
    close(simpleSocket);
    return 0;
}
