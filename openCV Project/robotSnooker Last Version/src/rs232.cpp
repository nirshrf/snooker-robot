#include "rs232.h"
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <tchar.h>


rs232::rs232(char* port)
{
    pcCommPort = port;
}

rs232::~rs232()
{
    //dtor
}
HANDLE rs232::Open()
{
	//DCB dcb;
    //HANDLE hCom;
    BOOL fSuccess;
	//char *pcCommPort = "COM5";
    hCom = CreateFile(_T(pcCommPort), //pcCommPort,
                     GENERIC_READ | GENERIC_WRITE,
                     0,    // must be opened with exclusive-access
                     0, // no security attributes
                     OPEN_EXISTING, // must use OPEN_EXISTING
                     FILE_ATTRIBUTE_NORMAL,    // not overlapped I/O
                     NULL  // hTemplate must be NULL for comm devices
                     );


   if (hCom == INVALID_HANDLE_VALUE)
   {
      // Handle the error.
       printf ("CreateFile failed with error %d.\n", GetLastError());
      }

   // Build on the current configuration, and skip setting the size
   // of the input and output buffers with SetupComm.
   fSuccess = GetCommState(hCom, &dcb);

   if (!fSuccess)
   {
     // Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
   }

   dcb.BaudRate = 115200;     // set the baud rate
   dcb.ByteSize = 8;             // data size, xmit, and rcv
   dcb.Parity = NOPARITY;        // no parity bit
   dcb.StopBits = ONESTOPBIT;    // one stop bit

   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess)
   {
      // Handle the error.
      printf ("SetCommState failed with error %d.\n", GetLastError());
   }


   printf ("Serial port %s successfully reconfigured.\n", pcCommPort);
   return hCom;

}

void rs232::Send(HANDLE hCom,char* data)
{
	printf("\n\n\n\n\n\n Start writting ! \n");

   WriteFile (hCom,              // Port handle
              data,              // Pointer to the data to write
              sizeof(data),                  // Number of bytes to write
              &dwNumBytesWritten, // Pointer to the number of bytes written
              NULL                    // Must be NULL for Windows Embedded CE
              );


            printf("\n Bytes Written to the terminal: ");
            for(j=0; j<dwNumBytesWritten; j++)
                 printf("%c",Buff[j]);


   printf("\n Byte length %d \n", dwNumBytesWritten);
}

char* rs232::Get()
{
 printf("\n\n\n\n\n\n Start reading !\n");

    ReadFile (hCom,                // Port handle
            Buff2,                // Pointer to data to read
            dwNumBytesWritten,     // Number of bytes to read
            &dwBytesTransferred,  // Pointer to number of bytes read
            NULL                  // Must be NULL for Windows Embeddded CE
            );


          for(j=0; j<dwNumBytesWritten; j++)
                 printf("%c",Buff2[j]);

                 printf("\n BytesRead from the terminal:%d \n",dwBytesTransferred);
				 return Buff2;
}

void rs232::Close(HANDLE hCom)
{
	CloseHandle(hCom);
}
