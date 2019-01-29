#include <iostream>
#include "RS232.h"

int main()
{
    const unsigned short port = 4u;
    const unsigned baud = 9600u;
    RS232::SerialConnection serConn;
    if(serConn.OpenPort(port, baud) == 0)
    {
        unsigned char byte = 1u;
        while(1)
        {
            /*//read a byte at a time
            serConn.ReadByte(port, byte);
            std::cout << (int)byte << std::endl;
            */
            //read a string
            unsigned char buffer[20];
            serConn.ReadByteArray(port, buffer, 20u);
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}
