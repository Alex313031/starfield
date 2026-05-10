#ifndef STARFIELD_STARS_H_
#define STARFIELD_STARS_H_

// clang-format off

#include "framework.h"

#include "constants.h"

//clang-format on

extern LONG nX[MAXSTARS];
extern LONG nY[MAXSTARS];
extern LONG nZ[MAXSTARS];

extern WORD wXScreen;
extern WORD wYScreen;
extern WORD wX2Screen;
extern WORD wY2Screen;

extern WORD wWarpSpeed; // Global WarpSpeed value
extern WORD wDensity;   // Global starfield density value

// Creates a star on the back buffer
void CreateStar(WORD wIndex);

// Custom rand function
void ssrand(DWORD dwSeed);

#endif // STARFIELD_STARS_H_
