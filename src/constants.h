#ifndef STARFIELD_CONSTANTS_H_
#define STARFIELD_CONSTANTS_H_

#include "framework.h"

// Perspective length
inline constexpr int SCOPE = 256;

#define MINWARP 0  // Minimum warp speed
#define MAXWARP 10 // Maximum warp speed

#define CLICKRANGE (MAXWARP - MINWARP) // Range for WarpSpeed scroll bar

inline constexpr int MINSTARS = 1;   // Minimum number of stars in field
inline constexpr int MAXSTARS = 500; // Maximum number of stars in field
inline constexpr int DEFSTARS = 25;  // Default number of stars in field

// Speed multiplier
inline constexpr int WARPFACTOR = 10; // Warp Factor 10 Mr. Sulu!

// Minimum window timer speed in milliseconds
inline constexpr int MINTIMERSPEED = static_cast<int>(16.7f); // ~60 FPS

// Rand helper macros
#define RAND(x)  ((rand() % (x)) + 1)
#define ZRAND(x) (rand() % (x))

// Avoid divide by zero
#define DIVIDE_SAFE(nNumber) ((0 == (nNumber)) ? 1 : (nNumber))

#endif // STARFIELD_CONSTANTS_H_
