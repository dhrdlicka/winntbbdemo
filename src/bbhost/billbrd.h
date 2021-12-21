#ifndef BILLBRD_H
#define BILLBRD_H

// Redraws BBMainHwnd
extern FARPROC BB_Refresh;

// Gets billboard panel HWND
extern FARPROC GetBBHwnd;

// Gets the main billboard HWND
extern FARPROC GetBBMainHwnd;

// Gets the panel count
extern FARPROC GetPanelCount;

// Show the billboard interface
extern FARPROC InitBillBoard;

// SendMessage to progress bar
extern FARPROC ProgressGaugeMsg;

// Set the information text under progress bar
extern FARPROC SetInfoText;

// Doesn't do anything, seems to be a leftover from Windows 9x
// Appears to be responsible for billboard switching during file copy stage
extern FARPROC SetProgress;

// Set the progress information above progress bar (Installing devices, etc.)
extern FARPROC SetProgressText;

// Set the setup stage (preparing, installing, finishing, etc.)
extern FARPROC SetStep;

// Set the time estimate (Setup will complete in XXX minutes)
extern FARPROC SetTimeEstimate;

// ShowWindow for progress bar
extern FARPROC ShowProgressGaugeWindow;

// Show billboard panels
extern FARPROC StartBillBoard;

// Hide billboard panels
extern FARPROC StopBillBoard;

// Close the billboard interface
extern FARPROC TermBillBoard;

int billbrd_load();
void billbrd_create();
void billbrd_close();

#endif