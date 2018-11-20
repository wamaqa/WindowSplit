#pragma once
#ifndef SERVICE_EXPORTS

#define EXPORT_LIBXLCRACK  _declspec(dllimport)
#pragma comment(lib,"..\\bin\\Service.lib")
#else
#define EXPORT_LIBXLCRACK  _declspec(dllexport)
#endif


EXPORT_LIBXLCRACK LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
EXPORT_LIBXLCRACK BOOL WINAPI SetHook();
EXPORT_LIBXLCRACK bool UnHook();