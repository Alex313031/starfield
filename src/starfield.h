#ifndef STARFIELD_STARFIELD_H_
#define STARFIELD_STARFIELD_H_

#include "stars.h"

#include "resource.h"

// Help IDs array
static DWORD aStarsDlgHelpIds[] = {static_cast<DWORD>(IDC_STATIC),
                                   static_cast<DWORD>(IDC_STATIC),
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

// Whether to fill background, possibly with custom color
static constexpr bool fill_bkgrnd = false;

// Any Screensaver with configurable options needs this function
WINBOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Same for this
WINBOOL WINAPI RegisterDialogClasses(HANDLE hInst);

// Uses legacy .ini functions from Win16 era.
void GetIniEntries();

#endif // STARFIELD_STARFIELD_H_
