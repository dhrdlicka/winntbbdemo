#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "billbrd.h"
#include "cmdline.h"
#include "hostwin.h"

FARPROC BB_Refresh;
FARPROC GetBBHwnd;
FARPROC GetBBMainHwnd;
FARPROC GetPanelCount;
FARPROC InitBillBoard;
FARPROC ProgressGaugeMsg;
FARPROC SetInfoText;
FARPROC SetProgress;
FARPROC SetProgressText;
FARPROC SetStep;
FARPROC SetTimeEstimate;
FARPROC ShowProgressGaugeWindow;
FARPROC StartBillBoard;
FARPROC StopBillBoard;
FARPROC TermBillBoard;

int billbrd_build;

int billbrd_query_version()
{
    int some_legacy_shit_that_windows_wants_to_write_a_zero_to;
    int verinfo_len = GetFileVersionInfoSizeA(filename, &some_legacy_shit_that_windows_wants_to_write_a_zero_to);
    void *verinfo = malloc(verinfo_len);
    
    GetFileVersionInfoA(filename, 0, verinfo_len, verinfo);

    VS_FIXEDFILEINFO *vsffi;
    int vsffi_len;
    
    VerQueryValue(verinfo, L"\\", &vsffi, &vsffi_len);

    int build = vsffi->dwFileVersionLS >> 16;

    free(verinfo);
    
    return build;
}

int billbrd_load()
{
    HMODULE h = LoadLibraryA(filename);
    if (h == NULL) {
        fprintf(stderr, "LoadLibrary returned 0x%X\n", GetLastError());
        return 0;
    }

    InitBillBoard = GetProcAddress(h, "InitBillBoard");
    if (InitBillBoard == NULL) {
        fprintf(stderr, "InitBillBoard is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
        return 0;
    }

    GetBBMainHwnd = GetProcAddress(h, "GetBBMainHwnd");
    if (GetBBMainHwnd == NULL) {
        fprintf(stderr, "GetBBMainHwnd is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
        return 0;
    }

    TermBillBoard = GetProcAddress(h, "TermBillBoard");
    if (TermBillBoard == NULL) {
        fprintf(stderr, "TermBillBoard is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
        return 0;
    }

    BB_Refresh = GetProcAddress(h, "BB_Refresh");
    if (BB_Refresh == NULL) {
        fprintf(stderr, "BB_Refresh is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    GetBBHwnd = GetProcAddress(h, "GetBBHwnd");
    if (GetBBHwnd == NULL) {
        fprintf(stderr, "GetBBHwnd is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    GetPanelCount = GetProcAddress(h, "GetPanelCount");
    if (GetPanelCount == NULL) {
        fprintf(stderr, "GetPanelCount is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    ProgressGaugeMsg = GetProcAddress(h, "ProgressGaugeMsg");
    if (ProgressGaugeMsg == NULL) {
        fprintf(stderr, "ProgressGaugeMsg is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    SetInfoText = GetProcAddress(h, "SetInfoText");
    if (SetInfoText == NULL) {
        fprintf(stderr, "SetInfoText is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    SetProgress = GetProcAddress(h, "SetProgress");
    if (SetProgress == NULL) {
        fprintf(stderr, "SetProgress is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    SetProgressText = GetProcAddress(h, "SetProgressText");
    if (SetProgressText == NULL) {
        fprintf(stderr, "SetProgressText is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    SetStep = GetProcAddress(h, "SetStep");
    if (SetStep == NULL) {
        fprintf(stderr, "SetStep is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    SetTimeEstimate = GetProcAddress(h, "SetTimeEstimate");
    if (SetTimeEstimate == NULL) {
        fprintf(stderr, "SetTimeEstimate is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    ShowProgressGaugeWindow = GetProcAddress(h, "ShowProgressGaugeWindow");
    if (ShowProgressGaugeWindow == NULL) {
        fprintf(stderr, "ShowProgressGaugeWindow is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    StartBillBoard = GetProcAddress(h, "StartBillBoard");
    if (StartBillBoard == NULL) {
        fprintf(stderr, "StartBillBoard is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    StopBillBoard = GetProcAddress(h, "StopBillBoard");
    if (StopBillBoard == NULL) {
        fprintf(stderr, "StopBillBoard is NULL (GetProcAddress returned 0x%X)\n", GetLastError());
    }

    billbrd_build = billbrd_query_version();

    return 1;
}

void billbrd_create()
{
    if(billbrd_build < 2296)
        InitBillBoard(hostwin_hwnd, L"");
    else
        InitBillBoard(hostwin_hwnd, L"", product_type);

    if(!IsChild(hostwin_hwnd, (HWND) GetBBMainHwnd()))
        SetParent((HWND) GetBBMainHwnd(), hostwin_hwnd);
}

void billbrd_close()
{
    TermBillBoard();
}