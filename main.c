#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <netdb.h>
#include "findPath.c"
#include "loadPath.c"

// userHandler is the method called within a thread each time a user accesses port 8000.
// cliSocket is taken in, a buffer is created to record any input incoming into the web server.
// After that, all the information is read into bytestore, and is fed through findPath.c to validate
// and then route the user to either a 404 response or a path found from the search.
void *userHandler(void *arg){
    int cliSocket = *(int*)arg;
    char buffer[500];
    ssize_t byteStore = read(cliSocket, buffer, 500 - 1);
    if (byteStore < 0) {
        perror("read");
        close(cliSocket);
        return NULL;
    }
    buffer[byteStore] = '\0';
    char* notFound = "404 Not found.";
    char *path = getPath(buffer);
    if (!pathExists(buffer)) {
        send(cliSocket, notFound, strlen(notFound), 0);
    } else {
        char* RESPONSE = response(path);
        printf("%s",RESPONSE);	
	send(cliSocket, RESPONSE, strlen(RESPONSE), 0);
        free(RESPONSE);
    }
    free(path);
    close(cliSocket);
        return NULL;
}

// main creates a server by initiating a server with a file descriptor, client socket,
// opt variable (to set the socket options), and addrinfo struct. The main method then
// creates a linkedlist of options for the script to parse through. It will set the
// IP and port to the structure so that it will emnable the server to listen from a
// specific port. It then goes through the serverInfo linkedlist, setting the socket
// to the family, socket options, and binding the sockets together. After the list
// is freed, it then sets listening on, and creates an en endless while loop which
// constantly is accepting any input which will be passed onto a thread and the
// process then ran.
int main(){
    int serverFd;
    int cliSocket;
    int opt = 1;
    struct addrinfo hints, *serverInfo, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "8000", &hints, &serverInfo);
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        serverFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        bind(serverFd, p->ai_addr, p->ai_addrlen);
        break;
    }
        freeaddrinfo(serverInfo);
    if (listen(serverFd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){
    	int cliSocket = accept(serverFd, NULL, NULL);
	pthread_t thread;
   	pthread_create(&thread, NULL, userHandler, &cliSocket);
    	pthread_detach(thread);
    }
    return 0;
}
