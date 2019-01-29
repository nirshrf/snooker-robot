#ifndef COMM_H
#define COMM_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class comm
{
    public:

        comm();
        void sendData(int input);
        virtual ~comm();
    protected:
    private:
        void *map_addr;
        volatile unsigned int *pointer;
};

#endif // COMM_H
