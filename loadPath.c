#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
// Response opens the path, retrieves the size of the filepath, and forms a response using
// sprintf. It then returns a string for main to utilize as a response.
char *response2(char *PATH) {
    FILE *file = fopen(PATH, "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *begin = malloc(200);
    if (begin == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(begin, "HTTP/1.1 200 OK\r\n\r\nContent-Length:%ld\r\n\r\n", size);
    char *buffer = malloc(strlen(begin) + size + 1);
    strcpy(buffer, begin);
    fread(buffer + strlen(begin), 1, size, file);
    buffer[strlen(begin) + size] = '\0';
    fclose(file);
    free(begin);
    printf("%s\n",buffer);
    return buffer;
}

char *response(char *PATH) {
    int file = open(PATH, O_RDONLY);
    off_t size = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);
    char *begin = malloc(200);
    sprintf(begin, "HTTP/1.1 200 OK\r\n\r\nContent-Length:%ld\r\n\r\n", size);
    char *buffer = malloc(strlen(begin) + size + 1);
    strcpy(buffer, begin);
    ssize_t bytes_read = read(file, buffer + strlen(begin), size);
    buffer[strlen(begin) + size] = '\0';
    close(file);
    free(begin);
    printf("%s\n",buffer);
    return buffer;
}



