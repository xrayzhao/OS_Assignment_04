#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int readFile(char *fileName) //use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if (!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

	char *command = NULL;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy, "\r\n");
	while (command != NULL)
	{
		command = strtok(NULL, "\r\n");
        printf("%s",command);
	}

    return 0;
}


int main(int argc, char *argv[])
{
    readFile(argv[1]);
    //testing
}
