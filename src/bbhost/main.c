#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <uxtheme.h>
#include "cmdline.h"
#include "hostwin.h"
#include "billbrd.h"
#include "hooks.h"

#define _LSTR(s) L ## s
#define LSTR(s) _LSTR(s)

#define PROGRESS_TEXT "This is a progress text set by SetProgressText."
#define TIME_ESTIMATE "This is a time estimate, which is set by SetTimeEstimate."
#define INFO_TEXT "This is an info text, which is set by SetInfoText."

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
        SetProgressText(ansi ? (void *) PROGRESS_TEXT : (void *) LSTR(PROGRESS_TEXT));

    if (SetTimeEstimate)
        SetTimeEstimate(ansi ? (void *) TIME_ESTIMATE : (void *) LSTR(TIME_ESTIMATE));

    if (SetInfoText)
        SetInfoText(ansi ? (void *) INFO_TEXT : (void *) LSTR(INFO_TEXT));

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