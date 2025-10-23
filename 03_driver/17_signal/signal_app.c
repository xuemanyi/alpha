#include "app.h"

static int fd = 0;

static void sigio_signal_func(int signum)
{
    int err = 0;
    unsigned int keyvalue = 0;

    err = read(fd, &keyvalue, sizeof(keyvalue));
    if (err < 0) {

    } else {
        printf("sigio signal key value = %d\n", keyvalue);
    }
}

int main(int argc, char const *argv[])
{
    int flags = 0;
    const char *filename = NULL;
    
    if (argc != 2) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("can't open file %s\n", filename);
        return -1;
    }

    signal(SIGIO, sigio_signal_func);

    fcntl(fd, F_SETOWN, getpid());
    flags = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFL, flags | FASYNC);

    while (1) {
        sleep(2);
    }

    close(fd);

    return 0;
}
