// apputils.c - common Application level utilities

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>

// strerror(3) like function for Win32
void AU_StrError(DWORD err, CHAR *buf, size_t buf_len)
{
	LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    StringCbCopy(buf,buf_len,lpMsgBuf);
	LocalFree(lpMsgBuf);
}