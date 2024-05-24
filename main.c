#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


#define BUF_SIZE 1024

void print_testvector(const char *FileName)
{

    int fd = open(FileName, O_RDONLY);
    int msg_bit = 0;

    if(fd == -1) {
        perror("error opening file");
	exit(EXIT_FAILURE);
    }

    FILE *file = fdopen(fd, "r");
    if(file == NULL) {
	perror("error fdopen fd");
	close(fd);
	exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];

    while(fgets(buf, sizeof(buf), file)) {

        if(buf[0] == '\n' || buf[0] == '#' || buf[0] == '[') {
		continue;
	}

	if(strncmp(buf, "Len = ", 6) == 0) {
	    sscanf(buf, "Len = %d", &msg_bit);
	    printf("Len      : %d\n", msg_bit);

	} else if(strncmp(buf, "Msg = ", 6) == 0) {
	        printf("Msg      : %s", buf + 6);

	} else if(strncmp(buf, "MD = ", 5) == 0) {
		printf("MD       : %s", buf + 5);


	}



    }

    fclose(file);
    close(fd);
}



int main(int argc, char **argv){


    const char *LongFileName = "SHA512LongMsg.rsp";
    const char *ShortFileName = "SHA512ShortMsg.rsp";

    printf("Long Msg\n");
    print_testvector(LongFileName);

    printf("\n");

    printf("Short Msg\n");
    print_testvector(ShortFileName);





    return 0;
}
