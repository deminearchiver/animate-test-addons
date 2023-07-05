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



typedef HRESULT(WINAPI *pDwmSetWindowAttribute)(HWND hWnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
typedef HRESULT(WINAPI *pDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* pMarInset);

typedef VOID(WINAPI *pRtlGetNtVersionNumbers)(DWORD* majorVersion, DWORD* minorVersion, DWORD* buildNumber);

bool enabled = false;
void TryEnable() {
    const auto RtlGetNtVersionNumbers = 
        reinterpret_cast<pRtlGetNtVersionNumbers>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetNtVersionNumbers"));
    DWORD major, build_number;
    RtlGetNtVersionNumbers(&major, nullptr, &build_number); 

    enabled = (major >= 10 && LOWORD(build_number) >= 22000);
}

HMODULE& GetDWMAPI() {
    static HMODULE dwmapi;
    return dwmapi;
}
HMODULE LoadDWMAPI() {
    auto& dwmapi = GetDWMAPI();
    if(!dwmapi) dwmapi = LoadLibraryW(L"dwmapi.dll");
    return dwmapi;
}

std::wstring GetWindowClassName(HWND hWnd) {
    wchar_t class_name[MAX_PATH + 1];
    GetClassNameW(hWnd, class_name, MAX_PATH + 1);
    return std::wstring(class_name);
}
bool SetWindowCaptionColor(HWND hWnd, COLORREF color) {
    auto& dwmapi = GetDWMAPI();
    if(!enabled || !dwmapi) return false;
    const auto DwmSetWindowAttribute = reinterpret_cast<pDwmSetWindowAttribute>(GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
    return DwmSetWindowAttribute ? DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(COLORREF)) == S_OK : false;
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
    if(code < 0 || !enabled) return CallNextHookEx(hook, code, wParam, lParam);

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

bool LoadTheme() {
    HMODULE handle;
    if(GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
        (LPCWSTR) &LoadTheme, 
        &handle
    ) == 0) return false;

    wchar_t file_name[MAX_PATH + 1];
    if(GetModuleFileNameW(handle, file_name, MAX_PATH + 1) == 0) return false;

    fs::path path(file_name, fs::path::generic_format);

    std::wregex regex(L"^(.:\\/Users\\/.+?\\/AppData)\\/Local\\/Adobe\\/Animate (\\d+)\\/(.+?)\\/Configuration\\/External Libraries");
    std::wsmatch matches;
    std::wstring path_string = path.generic_wstring();
    if(!std::regex_search(path_string, matches, regex)) return false;

    std::wstring appdata = matches[1].str();
    std::wstring version = matches[2].str();
    std::wstring locale = matches[3].str();
    
    fs::path roaming = appdata;
    roaming /= "Roaming/Adobe/Animate";
    roaming /= version;
    roaming /= "Application.xml";

    pugi::xml_document document;
    if(!document.load_file(roaming.generic_wstring().c_str())) return false;

    for(auto& child : document.child("prop.map").child("prop.list").children()) {
        if(!child.child("key") || !child.child("float")) continue;
        if(std::strcmp(child.child("key").text().as_string(), "ThemeBrightness") == 0) {
            theme_brightness = child.child("float").text().as_float() * 100;
            return true;
        }
    }
    return false;
}


ANIMATE_INIT(Init)
void Init() {
    TryEnable();
    if(enabled) {
        GetDWMAPI() = LoadLibraryW(L"dwmapi.dll");

        if(!LoadTheme()) MessageBoxW(nullptr, L"Could not load the user theme! Falling back to Dark.", L"animate11", MB_OK | MB_ICONERROR);
        hook = SetWindowsHookExW(WH_CALLWNDPROCRET, &CallWndRetProc, nullptr, GetCurrentThreadId());
    } else {
        MessageBoxW(nullptr, L"You must have Windows 11 21H2 or newer to use this addon!", L"animate11", MB_OK | MB_ICONERROR);
    }
}

ANIMATE_TERMINATE(Terminate)
int Terminate() {
    UnhookWindowsHookEx(hook);
    FreeLibrary(GetDWMAPI());
    return 0;
}