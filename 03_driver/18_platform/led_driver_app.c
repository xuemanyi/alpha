#include "app.h"

#define LEDOFF 0
#define LEDON  1

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    unsigned char databuf[2];

    if (argc != 3) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("file %s open failed\n", argv[1]);
        return -1;
    }

    databuf[0] = atoi(argv[2]);
    ret = write(fd, databuf, sizeof(databuf));
    if (ret < 0) {
        printf("led control failed\n");
        close(fd);
        return -1;
    }

    close(fd);
    
    return 0;
}
