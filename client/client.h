#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>

int clientInit();
int clientSendData(const char *data);
int clientReceiveData(char *buffer, int bufsize);

#endif // __CLIENT_H__
