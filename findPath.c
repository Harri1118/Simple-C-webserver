#include <stdbool.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// getPath retrieves the filepath from the input head. It returns
// the result afterwards.
char *getPath(char input[500]){
int i = 5;
int n = 0;
char Path[100];
while(input[i] != ' '){
Path[n] = input[i];
n++;
i++;
}
Path[n] = '\0';
char *fin = strdup(Path);
return fin;
}

// pathExists filters the header so that the filepath can be extracted
// from it. It then calls fileExists to validate whether the file
// exists or not.
bool pathExists(char input[500]){
	char *path = getPath(input);
	bool exists;
	if (access(path, F_OK) != -1)
		exists = true;
	else
		exists = false;
	free(path);
	if(exists == false)
        	return false;
	return true;
}
