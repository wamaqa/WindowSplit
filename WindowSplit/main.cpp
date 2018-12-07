#include <Windows.h>
#include "../Service/Service.h"
#include "../Service/SplitArea.h"

int main()
{
    SetHook();
    MessageBox(nullptr, "a", "a", 0);
    UnHook();
}


