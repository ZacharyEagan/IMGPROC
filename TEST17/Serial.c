#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "Serial.h"

#define BUFF_STD 64

int Find_Array_Coms( void ) {
    int fd;
    char *port = (char *)"/dev/ttyUSB0";
    int count = 0;

    while ((fd = open_Port(port)) <= 0 && count < 10) {
        printf("Port: %s empty\n", port);
        port[11] = port[11] + 1;
        count++;
    }

    if (fd > 0) {
        printf("Using Port %s\n", port);
    } else {
        printf("Error: Array not found\n");
    }

    return fd;
}

int INIT_Array (int fd) {
    char buff[BUFF_STD];

    /* clear buffer (internet says no better way for serial by USB) */
    while ((read_Port(fd, buff, 64)) > 0)
     ;

    write_Port (fd, (char *)"I");
    if (waitSelect(fd, 1000000) == 0) {
        printf("INIT_Array: Array took too long\n");
        return -1;
    }

    if ((readln_Port(fd, buff, 64)) > 0)
        if ((buff[0] == '0'))
            return 0;
        else
            printf("BUFF %s\n", buff);

    printf("INIT_ARRAY: Array not respoding to init\n");
    return -2;
}


int Array_Zero(int fd) { 
    char buff[BUFF_STD]; 
 
    if (!fd) { 
        printf("Array_Zero: port not found\n"); 
        return -1; 
    } 
    write_Port (fd, (char *) "I"); 
    if (waitSelect(fd, 1000000) == 0) { 
        printf("Array_Zero: Array took too long\n"); 
        return -2; 
    } 
 
    if ((readln_Port(fd, buff, BUFF_STD)) > 0) 
        if ((buff[0] == '0')) 
            return 0; 
 
    return -3; 
} 



int Array_Next(int fd) {
    char buff[BUFF_STD];

    if (!fd) {
        printf("Array_Zero: port not found\n");
        return -1;
    }
    write_Port (fd, (char *) "N");
    if (waitSelect(fd, 1000000) == 0) {
        printf("Array_Zero: Array took too long\n");
        return -2;
    }

    if ((readln_Port(fd, buff, BUFF_STD)) > 0)
            return ((int)buff[0] - 48);

    return -3;
}


int open_Port(char *port) {
    int fd;
    
    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (!fd) {
        perror("open_Port: port open error");
        return -1;
    }

    //fcntl(fd, F_SETFL, 0);

    if (INIT_Port(fd) < 0) {
        close (fd);
        fd = 0;;
    }

    return fd;
}

int INIT_Port(int fd) {
    struct termios opt;

    if (!fd) {
        perror("INIT_port: port not found");
        return -1;
    }
    
    /*get current settings*/
    tcgetattr(fd, &opt);

    /*baud to 9600*/
    cfsetispeed(&opt, B115200);
    cfsetospeed(&opt, B115200);

    /*set control to local*/
    opt.c_cflag |= (CLOCAL | CREAD);

    /*set word size to 8 bits*/
    opt.c_cflag &= ~CSIZE; /* zero the section */
    opt.c_cflag |= CS8; /* 8 bit word */

    /*set to paritdy style 8N1*/
    /*   opt.c_cflag &= ~PARENB;  no parity */
    /*   opt.c_cflag &= ~CSTOPB;  1 stop bit */ 

    opt.c_cflag |= PARENB; /* parity bit */
    opt.c_cflag |= CSTOPB; /* 2 stop bits */ 
    opt.c_cflag &= ~PARODD;/* Even Parity */

    /*Disable hardware flow control*/
    opt.c_cflag &= ~CRTSCTS;
    
    /*Raw Input mode (no echoing and no edditing*/
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHONL);
    opt.c_lflag |= IEXTEN;

    /*Disable parity checking*/
    /*opt.c_iflag &= ~(INPCK | ISTRIP);*/

    /*Enable parity checking, and strip parrity bit*/
    opt.c_iflag |= (INPCK | ISTRIP);

    /*Disable Software flow control*/
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);

    /*no output processing ie.  raw output mode*/
    opt.c_oflag &= ~OPOST;

    /*and finally update the options*/
    tcsetattr(fd, TCSANOW, &opt);

    return 0;
}

int write_Port(int fd, char *strn) {
    if (!fd) {
        perror("write_Port: port not found");
        return -1;
    }
    if (write (fd, strn, strlen(strn)) < 0) {
        perror("write_Port: write failed");
        return -2;
    }

    return 0;
}

int read_Port(int fd, char *buff, size_t size) {
    int count = 0;

    if (!fd) {
        perror("read_Port: port not found");
        return -1;
    }
    
    count = read(fd, buff, size - 1);
    buff[count] = '\0';

    return count;
}

int readln_Port(int fd, char *buff, size_t size) {
    int count = 0;
    int n;

    if (!fd) {
        perror("read_Port: port not found");
        return -1;
    }

    for (count = 0; count < size; count++) {
        buff[count] = '\0';
    }
    count = 0;

    do {

        n = read (fd, buff + count, 1);
        if (n > 0) 
            count++;
    }while (buff[count - 1] != '\n' && count < size - 1 && n != -1);
   
    buff[count] = '\0';

    return count;
}

int waitSelect(int port, long timeoutUS) {
    fd_set set;
    struct timeval tv;

    int numOut;
    
    tv.tv_sec = 0;
    tv.tv_usec = timeoutUS;

    FD_ZERO(&set);
    FD_SET(port, &set);

    

    if ((numOut = select(FD_SETSIZE, &set, NULL, NULL, &tv))== -1) {
        perror("select");
        return -1;
    }


    return numOut;
}




