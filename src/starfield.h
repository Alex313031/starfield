#ifndef STARFIELD_STARFIELD_H_
#define STARFIELD_STARFIELD_H_

#include "framework.h"

WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

WINBOOL WINAPI RegisterDialogClasses(HANDLE hInst);

void GetIniEntries();

void ssrand(DWORD dwSeed);

#endif // STARFIELD_STARFIELD_H_
