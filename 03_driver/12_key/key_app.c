#include "app.h"

#define KEY_VALUE 0XF0
#define INVAKEY   0X00

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    unsigned char keyvalue;

    if (argc != 2) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("file %s open failed\n", argv[1]);
        return -1;
    }

    while (1) {
        read(fd, &keyvalue, sizeof(keyvalue));
        if (keyvalue == KEY_VALUE) {
            printf("key0 press, value = %#X\n", keyvalue);
        }
    }

    ret = close(fd);
    if (ret < 0) {
        printf("file %s close failed\n");
        return -1;
    }
    
    return 0;
}
