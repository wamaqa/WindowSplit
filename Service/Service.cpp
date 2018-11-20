// Service.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Service.h"
#include "Windows.h"

#include "stdafx.h"
#include "Service.h"
#pragma data_seg("Shared")
HHOOK g_hHook = NULL;
DWORD g_dwThreadIdDIPS = 0;
HINSTANCE g_hInstDll = NULL;
#pragma data_seg()
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstDll = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

BOOL WINAPI SetHook()
{
	auto g_hook = SetWindowsHookEx(WH_KEYBOARD, GetMsgProc, g_hInstDll, 0);
	return g_hook != nullptr;
}

bool UnHook()
{
	return UnhookWindowsHookEx(g_hHook);
}

LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
	MessageBox(nullptr, L"a", L"a", 0);
	return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}
