#include "app.h"

#define CLOSE_CMD      (_IO(0XEF, 0x1))
#define OPEN_CMD       (_IO(0XEF, 0x2))
#define SET_PERIOD_CMD (_IO(0XEF, 0x3))

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    unsigned int cmd = 0;
    unsigned int arg = 0;
    unsigned char str[128] = {0};

    if (argc != 2) {
        printf("error usage\n");
        return -1;
    }

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("can't open file %s\n", filename);
        return -1;
    }

    while (1) {
        printf("input CMD:");
        ret = scanf("%d", &cmd);
        if (ret != 1) {
            gets(str);
        }

        if (cmd == 1) {
            cmd = CLOSE_CMD;
        }
        else if (cmd == 2) {
            cmd = OPEN_CMD;
        }
        else if (cmd == 3) {
            cmd = SET_PERIOD_CMD;
            printf("input timer period:");
            ret = scanf("%d", &arg);
            if (ret != 1) {
                gets(str);
            }
        }

        ioctl(fd, cmd, arg);
    }

    close(fd);
    
    return 0;
}
