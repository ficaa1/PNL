#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "hellioctl.h"

int main(void)
{
	char buf[MSG_SIZE];
	int fd;
	int ret;

	fd = open("/dev/hello", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	ret = ioctl(fd, HELLO, buf);
	printf("Valeur du buf : %s Valeur de retour : %d \n",buf ,ret);
	ret = ioctl(fd, WHO, "beer");
	ret = ioctl(fd, HELLO, buf);
	printf("Valeur du buf : %s Valeur de retour : %d \n",buf ,ret);
	return 0;
}
