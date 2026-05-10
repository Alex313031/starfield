// clang-format off

// Icons
// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
#define IDI_MAIN         100 // Keep 100
#define IDI_SMALL        101 // 16x16px

// Strings
#define idsName          200
#define idsDescription   201

// Options
#define IDC_SPEED         300 // Speed scrollbar
#define IDC_SPEED_SLOW    301 // "Slower" label
#define IDC_SPEED_FAST    302 // "Faster" label
#define IDC_DENSITY_LABEL 303 // "Density" label
#define IDC_DENSITY       304 // Density manual input edit control
#define IDC_DENSITYARROW  305 // Density arrow button
#define IDC_STARCOLOR     307 // Star color picker
#define IDC_BKGCOLOR      308 // Space color picker
#define IDC_DEFAULTS      309 // Reset all settings to defaults
#define IDC_STARSWATCH    310 // Filled rect next to Star Color button
#define IDC_BKGSWATCH     311 // Filled rect next to Space Color button

#define IDC_PREVIEW       400 // Preview button to launch ScreenSaverProc

#ifndef DLG_SCRNSAVECONFIGURE
 #define DLG_SCRNSAVECONFIGURE 2003 // Keep 2003
#endif

// Help IDs
#define IDH_DISPLAY_SCREENSAVER_STARFIELD_WARP    4216
#define IDH_DISPLAY_SCREENSAVER_STARFIELD_DENSITY 4217

// For resources to be loaded without an ID from the system.
#ifndef IDC_STATIC
 #define IDC_STATIC      -1
#endif // IDC_STATIC

// clang-format on

// Defined in scrnsave.h, but don't want to pull that in here.
#ifndef WS_GT
 #define WS_GT (WS_GROUP | WS_TABSTOP)
#endif
