#include "app.h"

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    char const *filename = NULL;
    unsigned char data = 0;

    if (argc != 2) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("can't open file %s \n", filename);
        return -1;
    }

    while (1) {
        ret = read(fd, &data, sizeof(data));
        if (ret < 0) {

        } else {
            if (data) {
                printf("key value = %#X\n", data);
            }
        }
    }

    close(fd);

    return ret;
}