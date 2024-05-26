#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <openssl/sha.h>

#define BUF_SIZE 51200

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



void verificate_testvector(const char *FileName)
{

    int fd = open(FileName, O_RDONLY);

    char *buf = (char *)malloc(BUF_SIZE);
    size_t buf_size = 0;
    int msg_bit = 0;
    int msg_byte = 0;
    int vector_count = 0;
    unsigned char *msg = NULL;

    unsigned char test_md[SHA512_DIGEST_LENGTH];
    unsigned char calculated_md[SHA512_DIGEST_LENGTH];


    SHA512_CTX ctx;


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


    while(fgets(buf, BUF_SIZE, file)) {

        if(buf[0] == '\n' || buf[0] == '#' || buf[0] == '[') {
		continue;
	}

	if(strncmp(buf, "Len = ", 6) == 0) {
	    sscanf(buf, "Len = %d", &msg_bit);
	    msg_byte = msg_bit / 8;
	    msg = (unsigned char *)malloc(msg_byte);

	} else if(strncmp(buf, "Msg = ", 6) == 0) {
		size_t msg_len = strlen(buf + 6);

		for(size_t i = 0; i < msg_byte; ++i) {
		    sscanf(buf + 6 + 2 * i, "%2hhx", &msg[i]);
		}


	} else if(strncmp(buf, "MD = ", 5) == 0) {
		for(size_t i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
		    sscanf(buf + 5 + 2 * i, "%2hhx", &test_md[i]);

		}


		SHA512_Init(&ctx);
		SHA512_Update(&ctx, msg, msg_byte);
		SHA512_Final(calculated_md, &ctx);

		printf("vector[%03d] ... ", vector_count);

		if(memcmp(test_md, calculated_md, SHA512_DIGEST_LENGTH) == 0) {
		    printf("pass\n");
		}

		else {


			printf("fail\n");
			printf("Len      : %d\n", msg_bit);
			printf("Msg      : ");
			for(int i = 0 ; i < SHA512_DIGEST_LENGTH; ++i) {

				printf("%02x", calculated_md[i]);

			}
			printf("\n");
			printf("but calculated MD is\n");
			printf("MD     : ");
			for(int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {

				printf("%02x", calculated_md[i]);

			}

			printf("\n");





		}

		++vector_count;
		free(msg);
		msg = NULL;


	}



    }


    free(buf);
    fclose(file);
    close(fd);




}



int main(int argc, char **argv){


    const char *LongFileName = "SHA512LongMsg.rsp";
    const char *ShortFileName = "SHA512ShortMsg.rsp";


    /*
    printf("Long Msg\n");
    print_testvector(LongFileName);

    printf("\n");

    printf("Short Msg\n");
    print_testvector(ShortFileName);
    */


    printf("LongMsg.rsp verificate\n");

    verificate_testvector(LongFileName);

    printf("ShortMsg.rsp verificate\n");

    verificate_testvector(ShortFileName);



    return 0;
}
