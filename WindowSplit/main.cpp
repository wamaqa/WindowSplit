#include<stdio.h>
#include <Windows.h>
#include "../Service/Service.h"
#include "../Service/SplitArea.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    if (!SetHook())
    {
        MessageBox(nullptr, L"ִ��config.exe���÷�������", L"����", 0);
        return 0;
    }
    MSG msg;
    GetMessage(&msg, nullptr, 0, 0);
    return true;
}




