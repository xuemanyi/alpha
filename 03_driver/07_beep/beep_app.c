#include "app.h"

#define BEEP_OFF  0
#define BEEP_ON   1

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    unsigned char databuf[1] = {0};

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
        printf("beep control failed\n");
        close(fd);
        return -1;
    }

    ret = close(fd);
    if (ret < 0) {
        printf("file %s close failed\n", argv[1]);
        return -1;
    }

    return 0;
}