// Starfield Simulator Screensaver Fork.

#include "starfield.h"

#include "strings.h"

WORD wXScreen;
WORD wYScreen;
WORD wX2Screen;
WORD wY2Screen;

WORD wWarpSpeed; // Global WarpSpeed value
WORD wDensity;   // Global starfield density value

/* This is the main window procedure to be used when the screen saver is
    activated in a screen saver mode ( as opposed to configure mode ). */
LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  static UINT_PTR wTimer;
  static WORD wWarp;
  static WORD wTimerSet = MINTIMERSPEED;
  static int nPassCount = 0;
  bool fHyperSpace = false;

  switch (message) {
    case WM_CREATE: {
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
      for (WORD wLoop = 0; wLoop < wDensity; wLoop++) {
        CreateStar(wLoop);
      }
      // Divide by WARP_STEP_DIVISOR so each tick advances the field a
      // smaller, smoother amount — compensates for the high-FPS timer.
      wWarp = (wWarpSpeed * WARPFACTOR + WARPFACTOR) / WARP_STEP_DIVISOR;

      wTimer = SetTimer(hWnd, 1, wTimerSet, NULL);
      break;
    }

    case WM_SIZE:
      wXScreen = LOWORD(lParam);
      wYScreen = HIWORD(lParam);
      break;

    case WM_TIMER: {
      MSG msg;
      DrawStars(hWnd, wWarp);

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

    case WM_ERASEBKGND: {
      // Fill the entire client area with the configured "outer space"
      // color so the field starts (and stays, where stars haven't drawn)
      // the user-chosen color rather than the system default.
      RECT rRect;
      GetClientRect(hWnd, &rRect);
      HBRUSH hBrush = CreateSolidBrush(g_bkg_color);
      FillRect(reinterpret_cast<HDC>(wParam), &rRect, hBrush);
      DeleteObject(hBrush);
      return 1l;
    }
    case WM_DESTROY:
      if (wTimer) {
        KillTimer(hWnd, wTimer);
      }
      break;
  }
  return DefScreenSaverProc(hWnd, message, wParam, lParam);
}

// Brushes backing the color swatches next to the picker buttons. Recreated
// whenever the underlying global color changes; freed on WM_DESTROY.
static HBRUSH g_hStarSwatch = nullptr;
static HBRUSH g_hBkgSwatch  = nullptr;

// Dialog procedure for the configuration dialog
WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  BOOL fError; // Error flag

  UINT wTemp; // temp int storage

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
      static const HICON kIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_SMALL));
      SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)kIcon);
      SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)kIcon);
      GetIniEntries();
      hWarpSpeed = GetDlgItem(hDlg, IDC_SPEED);
      hIDOK      = GetDlgItem(hDlg, IDOK);
      if (hIDOK != nullptr) {
        hDensity = GetDlgItem(hDlg, IDC_DENSITY);
        SendMessage(hDensity, EM_LIMITTEXT, 3, 0);

        SendDlgItemMessage(hDlg, IDC_DENSITYARROW, UDM_SETBUDDY, reinterpret_cast<WPARAM>(hDensity),
                           0);
        SendDlgItemMessage(hDlg, IDC_DENSITYARROW, UDM_SETRANGE, 0,
                           MAKELONG(lMaxScroll, lMinScroll));

        SetScrollRange(hWarpSpeed, SB_CTL, MINWARP, MINWARP + CLICKRANGE, FALSE);
        SetScrollPos(hWarpSpeed, SB_CTL, wWarpSpeed, TRUE);

        SetDlgItemInt(hDlg, IDC_DENSITY, wDensity, FALSE);
      }
      RefreshSwatches(hDlg);
      return true;
    case WM_CTLCOLORSTATIC: {
      // Paint each swatch's background with its corresponding color brush;
      // the static is empty-text so the brush fills the whole client area.
      const int id = GetDlgCtrlID(reinterpret_cast<HWND>(lParam));
      if (id == IDC_STARSWATCH) return reinterpret_cast<INT_PTR>(g_hStarSwatch);
      if (id == IDC_BKGSWATCH)  return reinterpret_cast<INT_PTR>(g_hBkgSwatch);
      break;
    }
    case WM_DESTROY:
      if (g_hStarSwatch) { DeleteObject(g_hStarSwatch); g_hStarSwatch = nullptr; }
      if (g_hBkgSwatch)  { DeleteObject(g_hBkgSwatch);  g_hBkgSwatch  = nullptr; }
      break;
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
        case IDC_DENSITY:
          if (HIWORD(wParam) == EN_UPDATE) {
            wTemp  = GetDlgItemInt(hDlg, IDC_DENSITY, &fError, FALSE);
            fError = ((wTemp <= MAXSTARS) && (wTemp >= MINSTARS));
            EnableWindow(GetDlgItem(hDlg, IDC_DENSITYARROW), fError);
            EnableWindow(GetDlgItem(hDlg, IDOK), fError);
          }
          break;
        case IDC_STARCOLOR:
        case IDC_BKGCOLOR: {
          // Common-dialog color picker. Custom colors are kept static so the
          // user's palette persists across both buttons within one session.
          static COLORREF custColors[16] = {0};
          COLORREF& target =
              (LOWORD(wParam) == IDC_STARCOLOR) ? g_star_color : g_bkg_color;
          CHOOSECOLOR cc{};
          cc.lStructSize  = sizeof(cc);
          cc.hwndOwner    = hDlg;
          cc.lpCustColors = custColors;
          cc.rgbResult    = target;
          cc.Flags        = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;
          if (ChooseColor(&cc)) {
            target = cc.rgbResult;
            RefreshSwatches(hDlg);
          }
          return true;
        }
        case IDC_PREVIEW: {
          if (!SaveSettings(hDlg)) {
            return true;
          }
          // Re-launch this .scr in fullscreen screensaver mode (/s) — the
          // same invocation the OS uses when the screensaver activates.
          // Config dialog stays open in the background; ESC/mouse exits.
          TCHAR szPath[MAX_PATH];
          if (GetModuleFileName(nullptr, szPath, MAX_PATH)) {
            TCHAR szCmd[MAX_PATH + 16];
            wsprintf(szCmd, TEXT("\"%s\" /s"), szPath);
            STARTUPINFO si{};
            si.cb = sizeof(si);
            PROCESS_INFORMATION pi{};
            if (CreateProcess(nullptr, szCmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si,
                              &pi)) {
              CloseHandle(pi.hProcess);
              CloseHandle(pi.hThread);
            }
          }
          return true;
        }
        case IDC_DEFAULTS:
          if (!SaveSettings(hDlg, true)) {
            MessageBox(hDlg, TEXT("Failed to save settings!"), TEXT("Config Error"), MB_OK | MB_ICONWARNING);
            return true;
          }
          RefreshSwatches(hDlg);
          return true;
        case IDOK:
          if (SaveSettings(hDlg)) {
            EndDialog(hDlg, LOWORD(wParam) == IDOK);
          }
          return true;
        case IDCANCEL:
          EndDialog(hDlg, LOWORD(wParam) == IDCANCEL);
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

bool SaveSettings(HWND hDlg, bool reset) {
  UINT dlg;
  BOOL fError;
  TCHAR szTemp[1025]; // Temporary string buffer (safe size for wsprintf)

  dlg = GetDlgItemInt(hDlg, IDC_DENSITY, &fError, FALSE);
  if (!fError) {
    return false;
  }

  if (reset) {
    // Restore every persisted setting to its compile-time default. We mutate
    // the in-memory globals first, then sync the dialog controls so the user
    // sees the change, then fall through into the normal save path so .ini
    // is rewritten with the defaults too.
    wDensity     = DEFSTARS;
    wWarpSpeed   = DEFWARP;
    g_star_color = RGB_STAR;
    g_bkg_color  = RGB_BLACK;
    SetDlgItemInt(hDlg, IDC_DENSITY, wDensity, FALSE);
    SetScrollPos(GetDlgItem(hDlg, IDC_SPEED), SB_CTL, wWarpSpeed, TRUE);
  }

  if (g_star_color == g_bkg_color) {
    MessageBox(hDlg, TEXT("Star color and space color cannot be the same."), TEXT("Colors Mismatch"), MB_OK | MB_ICONWARNING);
    return false;
  }

  wsprintf(szTemp, TEXT("%d"), dlg);
  const bool wroteDensity = WritePrivateProfileString(szAppName, szDensity, szTemp, szIniFile);
  wsprintf(szTemp, TEXT("%d"), wWarpSpeed);
  const bool wroteSpeed = WritePrivateProfileString(szAppName, szWarpSpeed, szTemp, szIniFile);
  wsprintf(szTemp, TEXT("%lu"), static_cast<unsigned long>(g_star_color));
  const bool wroteStarColor = WritePrivateProfileString(szAppName, szStarColor, szTemp, szIniFile);
  wsprintf(szTemp, TEXT("%lu"), static_cast<unsigned long>(g_bkg_color));
  const bool wroteBkgColor = WritePrivateProfileString(szAppName, szBkgColor, szTemp, szIniFile);
  return wroteDensity && wroteSpeed && wroteStarColor && wroteBkgColor;
}

// Load our settings from control.ini
static void GetIniEntries() {
  LoadString(hMainInstance, idsName, szName, CharSizeOf(szName));
  LoadString(hMainInstance, idsAppName, szAppName, CharSizeOf(szAppName));

  // Load Common Strings from stringtable...
  LoadString(hMainInstance, idsIniFile, szIniFile, CharSizeOf(szIniFile));
  LoadString(hMainInstance, idsScreenSaver, szScreenSaver, CharSizeOf(szScreenSaver));
  LoadString(hMainInstance, idsHelpFile, szHelpFile, CharSizeOf(szHelpFile));
  LoadString(hMainInstance, idsNoHelpMemory, szNoHelpMemory, CharSizeOf(szNoHelpMemory));

  wWarpSpeed =
      static_cast<WORD>(GetPrivateProfileInt(szAppName, szWarpSpeed, DEFWARP, szIniFile));
  if (wWarpSpeed > MAXWARP) {
    wWarpSpeed = DEFWARP;
  }

  wDensity = static_cast<WORD>(GetPrivateProfileInt(szAppName, szDensity, DEFSTARS, szIniFile));
  if (wDensity > MAXSTARS) {
    wDensity = MAXSTARS;
  }
  if (wDensity < MINSTARS) {
    wDensity = MINSTARS;
  }

  // COLORREF is 0x00BBGGRR (max 0x00FFFFFF), well below INT_MAX, so the
  // signed-int return from GetPrivateProfileInt round-trips cleanly.
  g_star_color = static_cast<COLORREF>(
      GetPrivateProfileInt(szAppName, szStarColor, RGB_STAR, szIniFile));
  g_bkg_color = static_cast<COLORREF>(
      GetPrivateProfileInt(szAppName, szBkgColor, RGB_BLACK, szIniFile));
}

// Rebuild swatch brushes from the current g_*_color globals and force the
// two static controls to repaint so they pick up the new brushes.
static void RefreshSwatches(HWND hDlg) {
  if (g_hStarSwatch) DeleteObject(g_hStarSwatch);
  if (g_hBkgSwatch)  DeleteObject(g_hBkgSwatch);
  g_hStarSwatch = CreateSolidBrush(g_star_color);
  g_hBkgSwatch  = CreateSolidBrush(g_bkg_color);
  InvalidateRect(GetDlgItem(hDlg, IDC_STARSWATCH), nullptr, TRUE);
  InvalidateRect(GetDlgItem(hDlg, IDC_BKGSWATCH), nullptr, TRUE);
}

// Original experimental code
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

// We use GetPrivateProfileInt, but this is a deprecated helper
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
