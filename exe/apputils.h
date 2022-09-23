#ifndef _APP_UTILS_H
#define _APP_UTILS_H

#include <windows.h>

// AU - App-Utils "namespace"

// strerror(3) like function for Win32
extern void AU_StrError(DWORD err, CHAR *buf, size_t buf_len);

#endif
