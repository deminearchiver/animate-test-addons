#include <animate/jsapi.h>
#include <Windows.h>

HHOOK gm_hook;
LRESULT WINAPI GetMessageProc(int code, WPARAM wParam, LPARAM lParam) {
    if(code < 0) return CallNextHookEx(gm_hook, code, wParam, lParam);
    MSG* info = reinterpret_cast<MSG*>(lParam);
    switch(info->message) {
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP: {
            if(info->wParam == VK_MENU || info->wParam == VK_LMENU || info->wParam == VK_RMENU) {
                info->message = WM_NULL;
            }
            break;
        }
    }
    return CallNextHookEx(gm_hook, code, wParam, lParam);
}

ANIMATE_INIT(Init)
void Init() {
    gm_hook = SetWindowsHookExW(WH_GETMESSAGE, &GetMessageProc, nullptr, GetCurrentThreadId());
}

ANIMATE_TERMINATE(Terminate)
int Terminate() {
    UnhookWindowsHookEx(gm_hook);
    return 0;
}