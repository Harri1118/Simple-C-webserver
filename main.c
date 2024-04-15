#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include "findPath.c"
#include "loadPath.c"

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

    // Process data
    // Example: Check if path exists
    char* notFound = "404 Not found.";
    //char *path = getPath(buffer);
    if (!pathExists(buffer)) {
        send(cliSocket, notFound, strlen(notFound), 0);
    } else {
        //printf("IS THIS WORKING HELLOOOOO\n");
        char *path = getPath(buffer);
        char *RESPONSE = response(path);
        //free(path);
        //char *RESPONSE = "test";
        send(cliSocket, RESPONSE, strlen(RESPONSE), 0);
        free(RESPONSE);
    }
    //free(path);
    close(cliSocket);
        return NULL;
}

int main(){
    // serverFd represents the file descriptors for the server socket
    int serverFd;
    // cliSocket represents the file descriptor for the client socket
    int cliSocket;
    // byteStore stores the amount of incoming bytes to the server.
    ssize_t byteStore;
    // opt set as option for setsockopt
    int opt = 1;
    // Structure to hold server address information
    struct sockaddr_in serverStruct;
    // serverlen represents the size of a server
    socklen_t serverlen = sizeof(serverStruct);
    // buffer to record the input length
    char buffer[500] = { 0 };

    // Create the socket file
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the socket behavior..
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // set the address family, IP address, and port to the socket.
    serverStruct.sin_family = AF_INET;
    serverStruct.sin_addr.s_addr = INADDR_ANY;
    serverStruct.sin_port = htons(8000);

    // socket with the server structure. Return an error otherwise
    if (bind(serverFd, (struct sockaddr*)&serverStruct, sizeof(serverStruct)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // start listening on the server.
    if (listen(serverFd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
    int cliSocket = accept(serverFd, (struct sockaddr*)&serverStruct, &serverlen);
    pthread_t thread;
    pthread_create(&thread, NULL, userHandler, &cliSocket);
    pthread_detach(thread);
    }
    return 0;
}
