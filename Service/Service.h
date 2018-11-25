#pragma once
#ifndef SERVICE_EXPORTS

#define EXPORT_LIBXLCRACK  _declspec(dllimport)
#pragma comment(lib,"..\\bin\\Service.lib")
#else
#define EXPORT_LIBXLCRACK  _declspec(dllexport)
#endif


LRESULT WINAPI GetKeyMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI GetMouseMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
EXPORT_LIBXLCRACK bool WINAPI SetHook();
EXPORT_LIBXLCRACK bool UnHook();
EXPORT_LIBXLCRACK bool Test();