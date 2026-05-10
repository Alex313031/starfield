#ifndef STARFIELD_STARFIELD_H_
#define STARFIELD_STARFIELD_H_

#include "framework.h"

VOID CreateStar(WORD wIndex);

[[maybe_unused]] LONG GetDlgItemLong(HWND hDlg, WORD wID, bool* pfTranslated, bool fSigned);

VOID GetIniEntries(VOID);

[[maybe_unused]] LONG GetPrivateProfileLong(LPTSTR pszApp, LPTSTR pszKey, LONG lDefault);

VOID ssrand(DWORD dwSeed);

WINBOOL WINAPI RegisterDialogClasses(HANDLE hInst);

WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif // STARFIELD_STARFIELD_H_
