#ifndef STARFIELD_CONSTANTS_H_
#define STARFIELD_CONSTANTS_H_

#include "framework.h"

// #include "uniconv.h"

#define SCOPE         256
#define MAXWARP       10                  // Maximum warp speed
#define MINWARP       0                   // Minimum warp speed
#define CLICKRANGE    (MAXWARP - MINWARP) // Range for WarpSpeed scroll bar
#define MINSTARS      10                  // Minimum number of stars in field
#define MAXSTARS      200                 // Maximum number of stars in field
#define WARPFACTOR    10                  // Warp Factor 10 Mr. Sulu!
#define SIZE          64
#define DEF_DENSITY   25 // Default number of stars in field
#define RAND(x)       ((rand() % (x)) + 1)
#define ZRAND(x)      (rand() % (x))
#define MINTIMERSPEED 50

#define DIVIDE_SAFE(nNumber) ((0 == (nNumber)) ? 1 : (nNumber))

#endif // STARFIELD_CONSTANTS_H_
