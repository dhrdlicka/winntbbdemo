#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <detours/detours.h>
#include "cmdline.h"
#include "hooks.h"
#include "hostwin.h"

BOOL (WINAPI *real_ClientToScreen)(HWND hWnd, LPPOINT lpPoint) = ClientToScreen;
HFONT (WINAPI *real_CreateFontIndirectW)(const LOGFONTW *lplf) = CreateFontIndirectW;
HWND (WINAPI *real_GetDesktopWindow)() = GetDesktopWindow;
int (WINAPI *real_GetDeviceCaps)(HDC hdc, int index) = GetDeviceCaps;
int (WINAPI *real_GetSystemMetrics)(int nIndex) = GetSystemMetrics;
BOOL (WINAPI *real_GetWindowRect)(HWND hWnd, LPRECT lpRect) = GetWindowRect;
BOOL (WINAPI *real_ScreenToClient)(HWND hWnd, LPPOINT lpPoint) = ScreenToClient;

BOOL WINAPI hooks_ClientToScreen(HWND hWnd, LPPOINT lpPoint)
{
    if (!real_ClientToScreen(hWnd, lpPoint)) return FALSE;

    if (IsChild(hostwin_hwnd, hWnd))
    {
        POINT topLeft = { 0, 0 };
        if (!real_ClientToScreen(hostwin_hwnd, &topLeft)) return FALSE;

        lpPoint->x -= topLeft.x;
        lpPoint->y -= topLeft.y;
    }

    return TRUE;
}

HFONT WINAPI hooks_CreateFontIndirectW(const LOGFONTW *lplf)
{
    LOGFONTW newFont;
    memcpy(&newFont, lplf, sizeof(newFont));
    newFont.lfQuality = NONANTIALIASED_QUALITY;
    return real_CreateFontIndirectW(&newFont);
}

HWND WINAPI hooks_GetDesktopWindow()
{
    return hostwin_hwnd;
}

int WINAPI hooks_GetDeviceCaps(HDC hdc, int index)
{
    switch (index) {
        case BITSPIXEL:
            return 4;
        case NUMCOLORS:
            return 16;
        default:
            return real_GetDeviceCaps(hdc, index);
    }
}

int WINAPI hooks_GetSystemMetrics(int nIndex)
{
    RECT rect;

    switch (nIndex) {
        case SM_CXSCREEN:
            GetClientRect(hostwin_hwnd, &rect);
            return rect.right - rect.left;
        case SM_CYSCREEN:
            GetClientRect(hostwin_hwnd, &rect);
            return rect.bottom - rect.top;
        default:
            return real_GetSystemMetrics(nIndex);
    }
}

BOOL WINAPI hooks_GetWindowRect(HWND hWnd, LPRECT lpRect)
{
    if (IsChild(hostwin_hwnd, hWnd))
    {
        if (!real_GetWindowRect(hWnd, lpRect)) return FALSE;

        POINT topLeft = { 0, 0 };
        if (!real_ClientToScreen(hostwin_hwnd, &topLeft)) return FALSE;

        lpRect->left -= topLeft.x;
        lpRect->right -= topLeft.x;

        lpRect->top -= topLeft.y;
        lpRect->bottom -= topLeft.y;

        return TRUE;
    }
    else return real_GetWindowRect(hWnd, lpRect);
}

BOOL WINAPI hooks_ScreenToClient(HWND hWnd, LPPOINT lpPoint)
{
    if (IsChild(hostwin_hwnd, hWnd))
    {
        POINT topLeft = { 0, 0 };
        if (!real_ClientToScreen(hostwin_hwnd, &topLeft)) return FALSE;

        lpPoint->x += topLeft.x;
        lpPoint->y += topLeft.y;
    }

    return real_ScreenToClient(hWnd, lpPoint);
}

int hooks_attach()
{
    DetourTransactionBegin();

    DetourAttach((PVOID *) &real_ClientToScreen, hooks_ClientToScreen);
    DetourAttach((PVOID *) &real_GetDesktopWindow, hooks_GetDesktopWindow);
    DetourAttach((PVOID *) &real_GetSystemMetrics, hooks_GetSystemMetrics);
    DetourAttach((PVOID *) &real_GetWindowRect, hooks_GetWindowRect);
    DetourAttach((PVOID *) &real_ScreenToClient, hooks_ScreenToClient);

    if (no_antialiasing)
        DetourAttach((PVOID *) &real_CreateFontIndirectW, hooks_CreateFontIndirectW);

    if (low_bpp)
        DetourAttach((PVOID *) &real_GetDeviceCaps, hooks_GetDeviceCaps);

    LONG ret = DetourTransactionCommit();
    if (ret) {
        fprintf(stderr, "DetourTransactionCommit returned 0x%X", ret);
        return 0;
    }

    return 1;
}

void hooks_detach()
{
    DetourTransactionBegin();

    DetourDetach((PVOID *) &real_ClientToScreen, hooks_ClientToScreen);
    DetourDetach((PVOID *) &real_GetDesktopWindow, hooks_GetDesktopWindow);
    DetourDetach((PVOID *) &real_GetSystemMetrics, hooks_GetSystemMetrics);
    DetourDetach((PVOID *) &real_GetWindowRect, hooks_GetWindowRect);
    DetourDetach((PVOID *) &real_ScreenToClient, hooks_ScreenToClient);

    if(no_antialiasing)
        DetourDetach((PVOID *) &real_CreateFontIndirectW, hooks_CreateFontIndirectW);

    if(low_bpp)
        DetourDetach((PVOID *) &real_GetDeviceCaps, hooks_GetDeviceCaps);

    DetourTransactionCommit();
}
