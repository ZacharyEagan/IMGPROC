#include <stdio.h>
#include <stdlib.h>
#include "Serial.h"

#define B_SIZE 64

int main(int argc, char **argv) {
    int fd;
    char buff[B_SIZE];
    int count;
    
    if (argc != 2) {
        printf("Usage: [PORT]\n");
        return 0;
    }

    if ((fd = open_Port(argv[1])) <= 0) {
        perror("main: fd was <= 0");
        return -1;
    }

    INIT_Port(fd);

    write_Port(fd, "U");

    while(1) {
        if (waitSelect(fd, 1000000) != 0) {
            if ((count = readln_Port(fd, buff, B_SIZE)) > 0) {
                printf("count: %d\tMessage: %s\n", count, buff);
            }
                
            if (buff[0] == 'U') {    
                if(write_Port(fd, "D")) {
                    printf("Write Fail\n");
                }
            } else {
                if (write_Port(fd, "U")) {
                    printf("Write Fail\n");
                }
           }
        }
    }
        
    return 0;
}
