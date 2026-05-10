#ifndef STARFIELD_STARFIELD_H_
#define STARFIELD_STARFIELD_H_

#include "framework.h"

WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

WINBOOL WINAPI RegisterDialogClasses(HANDLE hInst);

VOID ssrand(DWORD dwSeed);

VOID CreateStar(WORD wIndex);

VOID GetIniEntries(VOID);

#endif // STARFIELD_STARFIELD_H_
