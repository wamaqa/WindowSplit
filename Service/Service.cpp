// Service.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Service.h"
#include "Windows.h"
#include <cstdio>
#include <cstdlib>
#include <list>
#include <string>
#include <fstream>
#include "KeyBoardTag.h"
#include <locale>
#include <thread>
#include "SplitArea.h"
#pragma data_seg("Shared")

#define WM_CUSTOM_PAINT WM_USER + 50
HHOOK g_hHook = NULL;
std::list<HHOOK> *hooks;
DWORD g_dwThreadIdDIPS = 0;
HINSTANCE g_hInstDll = NULL;
std::ofstream  file;
HWND hWnd;
int i = 0;
static  std::list<std::wstring> *list = nullptr;
split_area_manage *split_area_manage;
KeyBoardTag *keyBoardTag;

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
        if (keyBoardTag == nullptr)
        {
            keyBoardTag = new KeyBoardTag;
        }
        if (split_area_manage == nullptr)
        {
            split_area_manage = new class split_area_manage;
            split_area_manage->init_from_config();
        }

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;

}

bool WINAPI SetHook()
{
    hooks = new std::list<HHOOK__*>;
    list = new std::list<std::wstring>;
    auto g_hook1 = SetWindowsHookEx(WH_KEYBOARD_LL, GetKeyMsgProc, g_hInstDll, 0);
    hooks->push_back(g_hook1);
    auto g_hook2 = SetWindowsHookEx(WH_MOUSE_LL, GetMouseMsgProc, g_hInstDll, 0);
    hooks->push_back(g_hook2);
    //hWnd = FindWindow(L"TestService", L"TestService");

    _wsetlocale(0, L"chs");
    file.open("log.txt");
    return true;
}

bool Test()
{
    POINT m_mouse;
    GetCursorPos(&m_mouse);
    wchar_t *p = new wchar_t[50];
    wsprintf(p, L"%s--code: %d; w:%d; x:%d, y :%d \n", L"鼠标", 0, 1, m_mouse.x, m_mouse.y);
    list->push_back(p);

    return true;
}

bool UnHook()
{

    if (hooks == nullptr) return false;
    try
    {
        for (auto hook : *hooks)
        {
            UnhookWindowsHookEx(g_hHook);
        }
    }
    catch (...)
    {
    }
    if (keyBoardTag != nullptr)
    {
        free(keyBoardTag);
        keyBoardTag = nullptr;
    }
    file.close();


    return true;
}

LRESULT WINAPI GetKeyMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
    switch (nCode)
    {
    case HC_ACTION:
    {
        KBDLLHOOKSTRUCT *kbdllhookstruct = (KBDLLHOOKSTRUCT *)lParam;
        switch (wParam)
        {
        case WM_KEYDOWN:
        {
            if (kbdllhookstruct->vkCode == VK_LCONTROL)
            {
                keyBoardTag->LCTRL = Down;
                if (kbdllhookstruct->time - keyBoardTag->LCTRLTime > 1000)
                {
                    i = 0;
                }
                keyBoardTag->LCTRLTime = kbdllhookstruct->time;
                if (i == 3)
                {
                    i = 0;
                    MessageBox(nullptr, L"ctrl", L"a", 0);
                }
            }
        }break;
        case WM_KEYUP:
        {
            if (kbdllhookstruct->vkCode == VK_LCONTROL)
            {
                keyBoardTag->LCTRL = Up;
                i++;
            }
        }
        }
    }
    }
    return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}

void  PlaceWindow(const POINT& m_mouse)
{
    const auto area = split_area_manage->find_area(m_mouse);
    if (area == nullptr) return;
    
    HWND hWnd = WindowFromPoint(m_mouse);
    HWND win = GetMainWindow(hWnd);
    MoveWindow(win, area->rect.left, area->rect.top, area->rect.right - area->rect.left, area->rect.bottom - area->rect.top, true);
    //wchar_t buff[50];
    //GetWindowText(win, buff, 50);
    //std::wstring a = buff;
    //std::string b;
    //std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
    //file << "鼠标" << m_mouse.x << "," << m_mouse.y << converter.to_bytes(buff) << std::endl;
}

LRESULT WINAPI GetMouseMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
    switch (wParam)
    {
    case WM_LBUTTONUP:
    {
        if (keyBoardTag->LCTRL == Down)
        {
            LPMSLLHOOKSTRUCT p_msllhookstruct = (LPMSLLHOOKSTRUCT)lParam;
            POINT m_mouse = p_msllhookstruct->pt;
            std::thread thread(PlaceWindow, m_mouse);
            thread.detach();
        }
    }
    }


    return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}

auto GetMainWindow(HWND hWnd) -> HWND
{
    HWND hp = GetParent(hWnd);
    if (hp == NULL)
    {
        return hWnd;
    }
    else
    {
        return GetMainWindow(hp);
    }
}
