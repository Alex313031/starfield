#ifndef STARFIELD_STARFIELD_H_
#define STARFIELD_STARFIELD_H_

#include "resource.h"
#include "stars.h"

// Help IDs array
static DWORD aStarsDlgHelpIds[] = {static_cast<DWORD>(IDC_STATIC),
                                   static_cast<DWORD>(IDC_STATIC),
                                   IDC_SPEED_SLOW,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   IDC_SPEED_FAST,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   IDC_SPEED,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP,
                                   IDC_DENSITY_LABEL,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   IDC_DENSITY,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   IDC_DENSITYARROW,
                                   IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY,
                                   0,
                                   0};

// Any Screensaver with configurable options needs this function
BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Same for this
BOOL WINAPI RegisterDialogClasses(HANDLE hInst);

// Password handling
BOOL WINAPI VerifyPassword(HWND hwnd);

// Saves our settings for next launch, if reset is true, settings are restored to defaults
bool SaveSettings(HWND hDlg, bool reset = false);

// Uses legacy .ini functions from Win16 era.
static void GetIniEntries();

static void RefreshSwatches(HWND hDlg);

#endif // STARFIELD_STARFIELD_H_
