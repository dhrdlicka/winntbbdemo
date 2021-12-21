#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <uxtheme.h>
#include "cmdline.h"
#include "hostwin.h"
#include "billbrd.h"
#include "hooks.h"

int main(int argc, char *argv[])
{
    SetThemeAppProperties(STAP_ALLOW_NONCLIENT);

    if (!cmdline_parse(argc, argv))
    {
        cmdline_usage();
        return -1;
    }

    if (!hooks_attach()) {
        return -2;
    }

    if (!billbrd_load()) {
        return -3;
    }

    hostwin_create();
    billbrd_create();

    hostwin_show();

    if (ShowProgressGaugeWindow)
        ShowProgressGaugeWindow(SW_SHOW);

    if (ProgressGaugeMsg) {
        ProgressGaugeMsg(PBM_SETRANGE32, 0, 100);
        ProgressGaugeMsg(PBM_SETPOS, 66, 0);
    }

    if (SetStep)
        SetStep(3);

    if (SetProgressText)
        SetProgressText(L"This is a progress text set by SetProgressText.");

    if (SetTimeEstimate)
        SetTimeEstimate(L"This is a time estimate, which is set by SetTimeEstimate.");

    if (SetInfoText)
        SetInfoText(L"This is an info text, which is set by SetInfoText.");

    if (StartBillBoard)
        StartBillBoard();

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0 ))
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    billbrd_close();
    hooks_detach();
}