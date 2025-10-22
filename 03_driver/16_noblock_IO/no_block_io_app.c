#include "app.h"

int main(int argc, char const *argv[])
{
    int fd = 0;
    int ret = 0;
    const char *filename = NULL;
    struct pollfd fds;
    fd_set readfds;
    struct timeval timeout;
    unsigned char data = 0;

    if (argc != 2) {
        printf("error usage\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        printf("can't open file %s \n");
        return -1;
    }

#if 0
    fds.fd = fd;
    fds.events = POLLIN;

    while (1) {
        ret = poll(&fds, 1, 500);
        if (ret) {
            ret = read(fd, &data, sizeof(data));
            if (ret < 0) {
                
            } else {
                if (data) {
                    printf("key value = %d\n", data);
                }
            }
        } else if (ret == 0) {
            
        } else if (ret < 0) {
            
        }
    }
#endif

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        ret = select(fd + 1, &readfds, NULL, NULL, &timeout);

        switch (ret) {
            case 0:    break;
            case -1:   break;
            default:
                if (FD_ISSET(fd, &readfds)) {
                    ret = read(fd, &data, sizeof(data));
                    if (ret < 0) {
                    } else {
                        if (data) {
                            printf("key value = %d\n", data);
                        }
                    }
                }
                break;
        }
    }

    close(fd);
    
    return 0;
}
