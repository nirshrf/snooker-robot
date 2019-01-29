#include "comm.h"

comm::comm()
{
    int fd;

    int size = 8;

    fd = open("/dev/uio0", O_RDWR);

    if(fd<0){
    //perror("Failed to open devfile");


    }
    else
    {
    map_addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map_addr == MAP_FAILED) {
    perror("Failed to mmap");
    exit(1);
    }

    //volatile unsigned int *pointer = map_addr;
    pointer = (volatile unsigned int*)map_addr;
    }

}
void comm::sendData(int input)
{
    *pointer = input;
}
comm::~comm()
{
    delete map_addr;
    delete pointer;
}
