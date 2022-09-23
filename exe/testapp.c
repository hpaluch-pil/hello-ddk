/*++

Copyright (c) 1990-98  Microsoft Corporation All Rights Reserved

Module Name:
    testapp.c
Environment:
    Win32 console multi-threaded application
--*/
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <sys\sioctl.h>

#include "apputils.h"


static const char *DEVICE_PATH = "\\\\.\\IoctlTest";
static char OutputBuffer[100];
static char InputBuffer[100];

VOID __cdecl
main(
    _In_ ULONG argc,
    _In_reads_(argc) PCHAR argv[]
    )
{
    HANDLE hDevice;
    BOOL bRc;
    ULONG bytesReturned;
    DWORD errNum = 0;
    CHAR ErrStr[256];

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    printf("Connecting to device '%s'...\n", DEVICE_PATH);

    // The DEVICE_PATH must exist otherwise driver is not installed
    hDevice = CreateFile(DEVICE_PATH,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        errNum = GetLastError();
        AU_StrError(errNum, ErrStr, sizeof(ErrStr));
        printf("Error: Unable to open device '%s': %d: %s\n", DEVICE_PATH, errNum, ErrStr);
        printf("Ensure that you have installed SYS driver.\n");
        return;
    }


    // Printing Input & Output buffer pointers and size
    printf("InputBuffer Pointer = %p, BufLength = %Iu\n", InputBuffer,
                        sizeof(InputBuffer));
    printf("OutputBuffer Pointer = %p BufLength = %Iu\n", OutputBuffer,
                                sizeof(OutputBuffer));

    // Performing METHOD_BUFFERED
    StringCbCopy(InputBuffer, sizeof(InputBuffer),
        "This String is from User Application; using METHOD_BUFFERED");

    printf("\nCalling DeviceIoControl METHOD_BUFFERED:\n");

    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    bRc = DeviceIoControl ( hDevice,
                            (DWORD) IOCTL_SIOCTL_METHOD_BUFFERED,
                            &InputBuffer,
                            (DWORD) strlen ( InputBuffer )+1,
                            &OutputBuffer,
                            sizeof( OutputBuffer),
                            &bytesReturned,
                            NULL
                            );

    if ( !bRc )
    {
        errNum = GetLastError();
        AU_StrError(errNum, ErrStr, sizeof(ErrStr));
        printf ( "Error in DeviceIoControl : %d: %s\n", errNum, ErrStr);
        return;

    }
    printf("    OutBuffer (%d): %s\n", bytesReturned, OutputBuffer);

    // Performing METHOD_NEITHER
    printf("\nCalling DeviceIoControl METHOD_NEITHER\n");

    StringCbCopy(InputBuffer, sizeof(InputBuffer),
               "This String is from User Application; using METHOD_NEITHER");
    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    bRc = DeviceIoControl ( hDevice,
                            (DWORD) IOCTL_SIOCTL_METHOD_NEITHER,
                            &InputBuffer,
                            (DWORD) strlen ( InputBuffer )+1,
                            &OutputBuffer,
                            sizeof( OutputBuffer),
                            &bytesReturned,
                            NULL
                            );

    if ( !bRc )
    {
        errNum = GetLastError();
        AU_StrError(errNum, ErrStr, sizeof(ErrStr));
        printf ( "Error in DeviceIoControl : %d: %s\n", errNum, ErrStr);
        return;
    }

    printf("    OutBuffer (%d): %s\n", bytesReturned, OutputBuffer);

    // Performing METHOD_IN_DIRECT
    printf("\nCalling DeviceIoControl METHOD_IN_DIRECT\n");

    StringCbCopy(InputBuffer, sizeof(InputBuffer),
               "This String is from User Application; using METHOD_IN_DIRECT");
    StringCbCopy(OutputBuffer, sizeof(OutputBuffer),
               "This String is from User Application in OutBuffer; using METHOD_IN_DIRECT");

    bRc = DeviceIoControl ( hDevice,
                            (DWORD) IOCTL_SIOCTL_METHOD_IN_DIRECT,
                            &InputBuffer,
                            (DWORD) strlen ( InputBuffer )+1,
                            &OutputBuffer,
                            sizeof( OutputBuffer),
                            &bytesReturned,
                            NULL
                            );

    if ( !bRc )
    {
        errNum = GetLastError();
        AU_StrError(errNum, ErrStr, sizeof(ErrStr));
        printf ( "Error in DeviceIoControl : %d: %s\n", errNum, ErrStr);
        return;
    }

    printf("    Number of bytes transfered from OutBuffer: %d\n",
                                    bytesReturned);
    // Performing METHOD_OUT_DIRECT
    printf("\nCalling DeviceIoControl METHOD_OUT_DIRECT\n");
    StringCbCopy(InputBuffer, sizeof(InputBuffer),
               "This String is from User Application; using METHOD_OUT_DIRECT");
    memset(OutputBuffer, 0, sizeof(OutputBuffer));
    bRc = DeviceIoControl ( hDevice,
                            (DWORD) IOCTL_SIOCTL_METHOD_OUT_DIRECT,
                            &InputBuffer,
                            (DWORD) strlen ( InputBuffer )+1,
                            &OutputBuffer,
                            sizeof( OutputBuffer),
                            &bytesReturned,
                            NULL
                            );

    if ( !bRc )
    {
        errNum = GetLastError();
        AU_StrError(errNum, ErrStr, sizeof(ErrStr));
        printf ( "Error in DeviceIoControl : %d: %s\n", errNum, ErrStr);
        return;
    }

    printf("    OutBuffer (%d): %s\n", bytesReturned, OutputBuffer);

    printf("Closing handle %p from device '%s'...\n", hDevice, DEVICE_PATH);
    // close the handle to the device.
    CloseHandle ( hDevice );

}
