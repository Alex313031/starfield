#ifndef STARFIELD_CONSTANTS_H_
#define STARFIELD_CONSTANTS_H_

#include "framework.h"

#define SIZE  64
#define SCOPE 256

#define MAXWARP 10 // Maximum warp speed
#define MINWARP 0  // Minimum warp speed

#define CLICKRANGE (MAXWARP - MINWARP) // Range for WarpSpeed scroll bar

#define MINSTARS    10  // Minimum number of stars in field
#define MAXSTARS    200 // Maximum number of stars in field
#define DEF_DENSITY 25  // Default number of stars in field

// Speec multiplier
#define WARPFACTOR  10 // Warp Factor 10 Mr. Sulu!

// Rand helper macros
#define RAND(x)  ((rand() % (x)) + 1)
#define ZRAND(x) (rand() % (x))

#define MINTIMERSPEED 50 // Minimum window timer speed

// Avoid divide by zero
#define DIVIDE_SAFE(nNumber) ((0 == (nNumber)) ? 1 : (nNumber))

#endif // STARFIELD_CONSTANTS_H_
