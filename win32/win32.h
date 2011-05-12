#ifndef _WIN32_H_
#define _WIN32_H_

#include <windows.h>

#if __cplusplus
extern "C"
{
#endif
	BOOL Win32_OpenFileDialog(LPTSTR filename, DWORD maxname);
	BOOL Win32_SaveFileDialog(LPTSTR filename, DWORD maxname);
#if __cplusplus
};
#endif

#endif