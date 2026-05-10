// Starfield Simulator Screensaver Fork.

#include "starfield.h"

#include "constants.h"
#include "strings.h"

TCHAR szWarpSpeed[] = TEXT("WarpSpeed"); // .INI WarpSpeed key
TCHAR szDensity[]   = TEXT("Density");   // .INI Density key

LONG nX[MAXSTARS];
LONG nY[MAXSTARS];
LONG nZ[MAXSTARS];

WORD wXScreen;
WORD wYScreen;
WORD wX2Screen;
WORD wY2Screen;

WORD wWarpSpeed; // Global WarpSpeed value
WORD wDensity;   // Global starfield density value

// Help IDs array
static DWORD aStarsDlgHelpIds[] = {static_cast<DWORD>(-1),
                                   static_cast<DWORD>(-1),
                                   ID_SPEED_SLOW,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   ID_SPEED_FAST,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   ID_SPEED,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   ID_DENSITY_LABEL,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   ID_DENSITY,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   ID_DENSITYARROW,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   0,
                                   0};

static constexpr bool fill_bkgrnd = false;

static void CreateStar(WORD wIndex);

/* This is the main window procedure to be used when the screen saver is
    activated in a screen saver mode ( as opposed to configure mode ). */
LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  RECT rRect;
  WORD wLoop;
  static UINT_PTR wTimer;
  static WORD wWarp;
  static WORD wTimerSet = MINTIMERSPEED;
  static WORD wCurrentWarp;
  static int nPassCount = 0;
  int nXTemp, nYTemp, nTemp;
  bool fHyperSpace = false;
  HDC hDC;

  switch (message) {
    case WM_CREATE:
      /* Do anything that you need to do when you initialize the window
         here... */
      GetIniEntries();
      ssrand(GetTickCount());

      /* Make sure we use the entire virtual desktop size for multiple
         displays... */
      wXScreen = static_cast<WORD>(reinterpret_cast<LPCREATESTRUCT>(lParam)->cx);
      wYScreen = static_cast<WORD>(reinterpret_cast<LPCREATESTRUCT>(lParam)->cy);

      wX2Screen = wXScreen / 2;
      wY2Screen = wYScreen / 2;
      for (wLoop = 0; wLoop < wDensity; wLoop++) {
        CreateStar(wLoop);
      }
      wWarp = wWarpSpeed * WARPFACTOR + WARPFACTOR; // ZRAND (((wWarpSpeed)*WARPFACTOR)+1)+1;

      wTimer = SetTimer(hWnd, 1, wTimerSet, NULL);
      break;

    case WM_SIZE:
      wXScreen = LOWORD(lParam);
      wYScreen = HIWORD(lParam);
      break;

    case WM_TIMER: {
      MSG msg;

      hDC = GetDC(hWnd);
      /* Begin to loop through each star, accelerating so it seems that
         we are traversing the starfield... */
      for (wLoop = 0; wLoop < wDensity; wLoop++) {
        nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                                  DIVIDE_SAFE(nZ[wLoop])) +
                 wX2Screen;
        nYTemp =
            static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) + wY2Screen;
        nTemp = static_cast<int>((SCOPE * WARPFACTOR - nZ[wLoop]) / (SIZE * WARPFACTOR)) + 1;
        PatBlt(hDC, nXTemp, nYTemp, nTemp, nTemp, BLACKNESS);

        if (wCurrentWarp < wWarp) {
          wCurrentWarp++;
        } else if (wCurrentWarp > wWarp) {
          wCurrentWarp--;
        }

        nZ[wLoop] = std::max(0, static_cast<int>(nZ[wLoop] - wCurrentWarp));
        if (!nZ[wLoop]) {
          CreateStar(wLoop);
        }

        nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                                  DIVIDE_SAFE(nZ[wLoop])) +
                 wX2Screen;
        nYTemp =
            static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) + wY2Screen;
        if ((nXTemp < 0 || nYTemp < 0) ||
            (nXTemp > static_cast<int>(wXScreen) || nYTemp > static_cast<int>(wYScreen))) {
          CreateStar(wLoop);
          nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                                    DIVIDE_SAFE(nZ[wLoop])) +
                   wX2Screen;
          nYTemp = static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) +
                   wY2Screen;
        }
        nTemp = static_cast<int>((SCOPE * WARPFACTOR - nZ[wLoop]) / (SIZE * WARPFACTOR)) + 1;
        PatBlt(hDC, nXTemp, nYTemp, nTemp, nTemp, WHITENESS);
      }
      ReleaseDC(hWnd, hDC);

      if (PeekMessage(&msg, hWnd, WM_TIMER, WM_TIMER, PM_REMOVE)) {
        // There is another WM_TIMER message in the queue.  We have
        // removed it, but now we want to adjust the timer a bit so
        // hopefully we won't get another WM_TIMER message before we
        // finish the screen update. (bug #8423)  TG:11/25/91
        wTimerSet += 10;
        SetTimer(hWnd, 1, wTimerSet, NULL);
        nPassCount = 0;
      } else {
        ++nPassCount;
      }

      if (nPassCount >= 100) {
        nPassCount = 0;
        wTimerSet -= 100;
        if (static_cast<short>(wTimerSet) < MINTIMERSPEED || fHyperSpace) {
          wTimerSet = MINTIMERSPEED;
        }
        SetTimer(hWnd, 1, wTimerSet, NULL);
      }
      break;
    }

    case WM_ERASEBKGND:
      /* If you want something put on the background, do it right here
          using wParam as a handle to a device context.  Remember to
          unrealize a brush if it is not a solid color.  If you do
          something here, you want to use the line:
              return 0l;
          So the program knows not to take the default action. Otherwise
          just use:
              break; */
      if (fill_bkgrnd) {
        GetClientRect(hWnd, &rRect);
        FillRect(reinterpret_cast<HDC>(wParam), &rRect,
                 static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
        return 0l;
      }
      break;
    case WM_DESTROY:
      if (wTimer) {
        KillTimer(hWnd, wTimer);
      }
      break;
  }
  return (DefScreenSaverProc(hWnd, message, wParam, lParam));
}

// Dialog procedure for the configuration dialog
WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  BOOL fError; // Error flag

  UINT wTemp;
  TCHAR szTemp[1025]; // Temporary string buffer (safe size for wsprintf)

  // static WORD wPause, wScroll;         // unused
  static HWND hWarpSpeed; // window handle of Speed scrollbar
  static HWND hIDOK;      // window handle of OK button
  static HWND hDensity;   // window handle of Density EditControl
  // HWND hSetPassword,       // window handle of SetPassword button - unused

  // static WORD wIncScroll = 1;    // density spin button parameters - unused
  // static WORD wStartScroll = 1;  // unused
  // static WORD wStartPause = 1;   // unused
  // static WORD wMaxScroll = 10;   // unused
  // static WORD wPauseScroll = 20; // unused
  static LONG lMinScroll = MINSTARS;
  static LONG lMaxScroll = MAXSTARS;

  switch (message) {
    case WM_INITDIALOG:
      GetIniEntries();
      hWarpSpeed = GetDlgItem(hDlg, ID_SPEED);
      hIDOK      = GetDlgItem(hDlg, IDOK);
      if (hIDOK != nullptr) {
        hDensity = GetDlgItem(hDlg, ID_DENSITY);
        SendMessage(hDensity, EM_LIMITTEXT, 3, 0);

        SendDlgItemMessage(hDlg, ID_DENSITYARROW, UDM_SETBUDDY, reinterpret_cast<WPARAM>(hDensity),
                           0);
        SendDlgItemMessage(hDlg, ID_DENSITYARROW, UDM_SETRANGE, 0,
                           MAKELONG(lMaxScroll, lMinScroll));

        SetScrollRange(hWarpSpeed, SB_CTL, MINWARP, MAXWARP, FALSE);
        SetScrollPos(hWarpSpeed, SB_CTL, wWarpSpeed, TRUE);

        SetDlgItemInt(hDlg, ID_DENSITY, wDensity, FALSE);
      }
      return true;
    case WM_HSCROLL:
      switch (LOWORD(wParam)) {
        case SB_LINEUP:
        case SB_PAGEUP:
          --wWarpSpeed;
          break;

        case SB_LINEDOWN:
        case SB_PAGEDOWN:
          ++wWarpSpeed;
          break;

        case SB_THUMBPOSITION:
          wWarpSpeed = HIWORD(wParam);
          break;

        case SB_TOP:
          wWarpSpeed = MINWARP;
          break;

        case SB_BOTTOM:
          wWarpSpeed = MAXWARP;
          break;

        case SB_THUMBTRACK:
        case SB_ENDSCROLL:
          return true;
          break;
      }
      if (static_cast<int>(static_cast<short>(wWarpSpeed)) <= MINWARP) {
        wWarpSpeed = MINWARP;
      }
      if (static_cast<int>(wWarpSpeed) >= MAXWARP) {
        wWarpSpeed = MAXWARP;
      }
      SetScrollPos(reinterpret_cast<HWND>(lParam), SB_CTL, wWarpSpeed, TRUE);
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_DENSITY:
          if (HIWORD(wParam) == EN_UPDATE) {
            wTemp  = GetDlgItemInt(hDlg, ID_DENSITY, &fError, FALSE);
            fError = ((wTemp <= MAXSTARS) && (wTemp >= MINSTARS));
            EnableWindow(GetDlgItem(hDlg, ID_DENSITYARROW), fError);
            EnableWindow(GetDlgItem(hDlg, IDOK), fError);
          }
          break;
        case IDOK:
          wTemp = GetDlgItemInt(hDlg, ID_DENSITY, &fError, FALSE);
          wsprintf(szTemp, TEXT("%d"), wTemp);
          WritePrivateProfileString(szAppName, szDensity, szTemp, szIniFile);
          wsprintf(szTemp, TEXT("%d"), wWarpSpeed);
          WritePrivateProfileString(szAppName, szWarpSpeed, szTemp, szIniFile);
          EndDialog(hDlg, LOWORD(wParam) == IDOK);
          return true;
        case IDCANCEL:
          EndDialog(hDlg, LOWORD(wParam) == IDOK);
          return true;
      }
      break;
    case WM_HELP: // F1
      WinHelp(static_cast<HWND>(reinterpret_cast<LPHELPINFO>(lParam)->hItemHandle), szHelpFile,
              HELP_WM_HELP, reinterpret_cast<ULONG_PTR>(reinterpret_cast<LPSTR>(aStarsDlgHelpIds)));
      break;
    case WM_CONTEXTMENU:
      WinHelp(reinterpret_cast<HWND>(wParam), szHelpFile, HELP_CONTEXTMENU,
              reinterpret_cast<ULONG_PTR>(reinterpret_cast<LPSTR>(aStarsDlgHelpIds)));
      break;
    default:
      break;
  }
  return false;
}

/* This procedure is called right before the dialog box above is created in
   order to register any child windows that are custom controls.  If no
   custom controls need to be registered, then simply return TRUE. */
WINBOOL WINAPI RegisterDialogClasses(HANDLE hInst) {
  InitCommonControls();
  return true;
}

static void CreateStar(WORD wIndex) {
  nX[wIndex] =
      wXScreen ? static_cast<LONG>(static_cast<int>(ZRAND(wXScreen)) - static_cast<int>(wX2Screen))
               : 0;
  nY[wIndex] =
      wXScreen ? static_cast<LONG>(static_cast<int>(ZRAND(wYScreen)) - static_cast<int>(wY2Screen))
               : 0;
  nZ[wIndex] = SCOPE * WARPFACTOR;
}

[[maybe_unused]] LONG GetDlgItemLong(HWND hDlg, WORD wID, bool* pfTranslated, bool fSigned) {
  TCHAR szTemp[20];
  LPTSTR pszTemp;
  LONG lTemp = 0l;
  bool fNegative;

  if (!GetDlgItemText(hDlg, wID, szTemp, CharSizeOf(szTemp))) {
    goto GetDlgItemLongError;
  }

  szTemp[19] = TEXT('\0');
  pszTemp    = szTemp;
  while (*pszTemp == TEXT(' ') || *pszTemp == TEXT('\t')) {
    pszTemp++;
  }
  if ((!fSigned && *pszTemp == TEXT('-')) || !*pszTemp) {
    goto GetDlgItemLongError;
  }
  fNegative = (*pszTemp == TEXT('-'));
  while (*pszTemp >= TEXT('0') && *pszTemp <= TEXT('9')) {
    lTemp = lTemp * 10l + static_cast<LONG>(*(pszTemp++) - TEXT('0'));
  }
  if (*pszTemp) {
    goto GetDlgItemLongError;
  }
  if (fNegative) {
    lTemp *= -1;
  }
  *pfTranslated = true;
  return lTemp;

GetDlgItemLongError:
  *pfTranslated = false;
  return 0l;
}

[[maybe_unused]] LONG GetPrivateProfileLong(LPTSTR pszApp, LPTSTR pszKey, LONG lDefault) {
  LONG lTemp = 0l;
  TCHAR szTemp[20];
  LPTSTR pszTemp;

  if (!GetPrivateProfileString(pszApp, pszKey, TEXT(""), szTemp, CharSizeOf(szTemp), szIniFile)) {
    return lDefault;
  }

  szTemp[19] = TEXT('\0');
  pszTemp    = szTemp;
  while (*pszTemp >= TEXT('0') && *pszTemp <= TEXT('9')) {
    lTemp = lTemp * 10l + static_cast<LONG>(*(pszTemp++) - TEXT('0'));
  }
  if (*pszTemp) {
    return lDefault;
  }
  return lTemp;
}

void GetIniEntries() {
  LoadString(hMainInstance, idsName, szName, CharSizeOf(szName));
  LoadString(hMainInstance, idsAppName, szAppName, CharSizeOf(szAppName));

  // Load Common Strings from stringtable...
  LoadString(hMainInstance, idsIniFile, szIniFile, CharSizeOf(szIniFile));
  LoadString(hMainInstance, idsScreenSaver, szScreenSaver, CharSizeOf(szScreenSaver));
  LoadString(hMainInstance, idsHelpFile, szHelpFile, CharSizeOf(szHelpFile));
  LoadString(hMainInstance, idsNoHelpMemory, szNoHelpMemory, CharSizeOf(szNoHelpMemory));

  wWarpSpeed = static_cast<WORD>(
      GetPrivateProfileInt(szAppName, szWarpSpeed, MINWARP + ((MAXWARP - MINWARP) / 2), szIniFile));
  if (wWarpSpeed > MAXWARP) {
    wWarpSpeed = MINWARP + ((MAXWARP - MINWARP) / 2);
  }

  wDensity = static_cast<WORD>(GetPrivateProfileInt(szAppName, szDensity, DEFSTARS, szIniFile));
  if (wDensity > MAXSTARS) {
    wDensity = MAXSTARS;
  }
  if (wDensity < MINSTARS) {
    wDensity = MINSTARS;
  }
}

void ssrand(DWORD dwSeed) {
  ::srand(static_cast<unsigned int>(dwSeed));
}
