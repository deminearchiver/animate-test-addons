#include <jsapi/api.h>
#include <Windows.h>
#include <dwmapi.h>
#include <string>

std::wstring GetWindowClassName(HWND hWnd) {
    wchar_t class_name[MAX_PATH + 1];
    GetClassNameW(hWnd, class_name, MAX_PATH + 1);
    return std::wstring(class_name);
}

HRESULT SetWindowCaptionColor(HWND hWnd, COLORREF color) {
    return DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(COLORREF));
}

HHOOK hook;
LRESULT WINAPI CallWndRetProc(int code, WPARAM wParam, LPARAM lParam) {
    if(code < 0) return CallNextHookEx(hook, code, wParam, lParam);

    CWPRETSTRUCT* info = reinterpret_cast<CWPRETSTRUCT*>(lParam);
    switch(info->message) {
        case WM_CREATE: {
            if(!IsWindow(info->hwnd)) break;

            LONG style = GetWindowLongW(info->hwnd, GWL_STYLE);
            if((style & WS_CAPTION) == WS_CAPTION || (style & WS_DLGFRAME) == WS_DLGFRAME) {
                std::wstring class_name = GetWindowClassName(info->hwnd);
                if(class_name == L"PLUGPLUG_UI_NATIVE_WINDOW_CLASS_NAME") SetWindowCaptionColor(info->hwnd, 0x1b1b1b);
                else SetWindowCaptionColor(info->hwnd, 0x333333);
            }
            break;
        }
    }
    
    return CallNextHookEx(hook, code, wParam, lParam);
}

ANIMATE_INIT(Init)
void Init(animate::jsapi::Environment env) {
    // MessageBoxW(nullptr, L"Addon loaded!", L"animate11", MB_OK | MB_ICONINFORMATION);
    hook = SetWindowsHookExW(WH_CALLWNDPROCRET, &CallWndRetProc, nullptr, GetCurrentThreadId());
}

ANIMATE_TERMINATE(Terminate)
int Terminate() {
    UnhookWindowsHookEx(hook);
    return 0;
}