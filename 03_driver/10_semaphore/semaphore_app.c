#include "app.h"

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    unsigned char cnt = 0;
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
        printf("LED control failed\n");
        close(fd);
        return -1;
    }

    while (1) {
        sleep(5);
        cnt++;
        printf("app running times: %d\n", cnt);

        if (cnt >= 5) {
            break;
        }
    }

    printf("app running finished\n");
    ret = close(fd);
    if (ret < 0) {
        printf("file %d close failed\n", fd);
        return -1;
    }

    return 0;
}
