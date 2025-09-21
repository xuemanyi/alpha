#include "app.h"

static char usrdata[] = {"user data"};

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    char *filename = NULL;
    char readbuf[128] = {0};
    char writebuf[128] = {0};
    
    if (argc != 3) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];

    fd = open(filename, O_RDWR);
    if (-1 == fd) {
        printf("can't open file: %s\n", filename);
        return -1;
    }

    if (1 == atoi(argv[2])) {
        ret = read(fd, readbuf, 50);
        if (ret < 0) {
            printf("read file %s failed\n", filename);
        } else {
            printf("read data: %s\n", readbuf)
        }
    }

    if (2 == atoi(argv[2])) {
        memcpy(writebuf, usrdata, sizeof(usrdata));
        
        ret = write(fd, writebuf, 50);
        if (ret < 0)
        {
            printf("write file %s failed\n", filename);
        }
    }

    ret = close(fd);
    if (ret < 0)
    {
        printf("close file(%s) failed\n", filename);
    }
    
    return 0;
}
