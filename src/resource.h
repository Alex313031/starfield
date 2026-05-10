// Icons
// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
#define IDI_MAIN         100 // Keep 100
#define IDI_SMALL        101 // 16x16px

// Strings
#define idsName          200
#define idsDescription   201
//#define idsIniFile       202
//#define idsScreenSaver   203
//#define idsAppName       204
//#define idsNoHelpMemory  205
//#define idsHelpFile      206
//#define idsDefKeyword		 207

// Options
#define ID_SPEED         300
#define ID_DENSITY       301
#define ID_DENSITYARROW  302
#define ID_SPEED_SLOW    304
#define ID_SPEED_FAST    305
#define ID_DENSITY_LABEL 306

// Dialogs
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

// Defined in scrnsave.h, but don't want to pull that in here.
#ifndef WS_GT
 #define WS_GT (WS_GROUP | WS_TABSTOP)
#endif
