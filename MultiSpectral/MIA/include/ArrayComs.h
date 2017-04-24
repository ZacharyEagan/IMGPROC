#ifndef SERIAL_DEF
#define SERIAL_DEF
#include <stdlib.h>

using namespace std;

int INIT_ARRAY(int *fd_array);
int Find_Array_Coms( void );
int INIT_Array(int fd);
int Array_Zero(int fd);
int Array_Next(int fd);
int Array_Refresh(int fd);
int INIT_Port(int fd);
int open_Port(char *port);
int write_Port(int fd, char *strn);
int read_Port(int fd, char *buff, size_t size);

/*Warning this is a bocking function*/
int readln_Port(int fd, char *buff, size_t size);

int waitSelect(int fd, long timeoutUS);



#endif
