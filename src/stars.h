#ifndef STARFIELD_STARS_H_
#define STARFIELD_STARS_H_

// clang-format off

#include "framework.h"

#include "constants.h"

//clang-format on

// Out star and background ("space") colors
extern COLORREF g_star_color;
extern COLORREF g_bkg_color;

// Avoid divide by zero
#define DIVIDE_SAFE(nNumber) ((0 == (nNumber)) ? 1 : (nNumber))

extern LONG nX[MAXSTARS];
extern LONG nY[MAXSTARS];
extern LONG nZ[MAXSTARS];

extern WORD wXScreen;
extern WORD wYScreen;
extern WORD wX2Screen;
extern WORD wY2Screen;

extern WORD wWarpSpeed; // Global WarpSpeed value
extern WORD wDensity;   // Global starfield density value

// Handles star movement
void DrawStars(HWND hWnd, WORD warpfactor);

// Creates a star on the back buffer
void CreateStar(WORD wIndex);

// Custom rand function
void ssrand(DWORD dwSeed);

#endif // STARFIELD_STARS_H_
