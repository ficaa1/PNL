#include <linux/ioctl.h>
#pragma once


#define DEV_NAME "hello"
#define IO_MAGIC 'N'
#define MSG_SIZE 15
#define ADAPT_SIZE 10
#define HELLO _IOR(IO_MAGIC, 0, char*)
#define WHO _IOW(IO_MAGIC, 1, char*) 


