#include <animate/jsapi.h>
#include <Windows.h>
#include <dwmapi.h>
#include <filesystem>
#include <pugixml.hpp>
#include <regex>
#include <string>
#include <cstring>
#include <map>

namespace fs = std::filesystem;

std::wstring GetWindowClassName(HWND hWnd) {
    wchar_t class_name[MAX_PATH + 1];
    GetClassNameW(hWnd, class_name, MAX_PATH + 1);
    return std::wstring(class_name);
}

HRESULT SetWindowCaptionColor(HWND hWnd, COLORREF color) {
    return DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(COLORREF));
}

std::map<int, std::pair<COLORREF, COLORREF>> colors = {
    { 0, { 0x1f1f1f, 0x1b1b1b }},
    { 25, { 0x333333, 0x1b1b1b }},
    { 75, { 0xf5f5f5, 0xfdfdfd }},
    { 100, { 0xffffff, 0xfdfdfd }}
};
int theme_brightness = 25;

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
                if(class_name == L"PLUGPLUG_UI_NATIVE_WINDOW_CLASS_NAME") {
                    SetWindowCaptionColor(info->hwnd, colors[theme_brightness].second);
                } else {
                    SetWindowCaptionColor(info->hwnd, colors[theme_brightness].first);
                }
            }
            break;
        }
    }
    
    return CallNextHookEx(hook, code, wParam, lParam);
}

void LoadTheme() {
    HMODULE handle;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR) &LoadTheme, &handle);
    wchar_t file_name[MAX_PATH + 1];
    GetModuleFileNameW(handle, file_name, MAX_PATH + 1);

    fs::path path(file_name, fs::path::generic_format);
    std::wregex regex(L"^(.:\\/Users\\/.+?\\/AppData)\\/Local\\/Adobe\\/Animate (\\d+)\\/(.+?)\\/Configuration\\/External Libraries");
    std::wsmatch matches;

    std::wstring path_string = path.generic_wstring();

    if(std::regex_search(path_string, matches, regex)) {
        std::wstring appdata = matches[1].str();
        std::wstring version = matches[2].str();
        std::wstring locale = matches[3].str();
        

        fs::path roaming = appdata;
        roaming /= "Roaming/Adobe/Animate";
        roaming /= version;
        roaming /= "Application.xml";

        pugi::xml_document document;
        if(document.load_file(roaming.generic_wstring().c_str())) {
            for(auto& child : document.child("prop.map").child("prop.list").children()) {
                if(!child.child("key") || !child.child("float")) continue;
                if(std::strcmp(child.child("key").text().as_string(), "ThemeBrightness") == 0) theme_brightness = child.child("float").text().as_float() * 100;
            }
            MessageBoxW(nullptr, std::to_wstring(theme_brightness).c_str(), L"bright", MB_OK | MB_ICONEXCLAMATION);
        }
    }
}

ANIMATE_INIT(Init)
void Init() {
    LoadTheme();
    hook = SetWindowsHookExW(WH_CALLWNDPROCRET, &CallWndRetProc, nullptr, GetCurrentThreadId());
}

ANIMATE_TERMINATE(Terminate)
int Terminate() {
    UnhookWindowsHookEx(hook);
    return 0;
}