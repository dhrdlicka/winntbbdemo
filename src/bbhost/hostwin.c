#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>
#include "cmdline.h"
#include "hostwin.h"

HWND hostwin_hwnd;

static LRESULT CALLBACK hostwin_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) 
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void hostwin_create()
{
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = hostwin_proc;
    wc.lpszClassName = L"BBHost";
    wc.hInstance = GetModuleHandle(NULL);

    RegisterClass(&wc);

    hostwin_hwnd = CreateWindowEx(
        WS_EX_COMPOSITED,
        L"BBHost", L"BBHost",
        WS_BORDER | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL, NULL,
        GetModuleHandle(NULL),
        NULL
    );

    RECT rect = { 0, 0, width, height };

    AdjustWindowRectEx(
        &rect,
        GetWindowLong(hostwin_hwnd, GWL_STYLE),
        GetMenu(hostwin_hwnd) != NULL,
        GetWindowLong(hostwin_hwnd, GWL_EXSTYLE));

    SetWindowPos(hostwin_hwnd, HWND_TOP, 0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        SWP_NOMOVE);
}

void hostwin_show()
{
    ShowWindow(hostwin_hwnd, SW_SHOW);
}