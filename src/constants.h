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

// Default warp speed: midpoint of the [MINWARP, MAXWARP] range.
inline constexpr int DEFWARP = MINWARP + ((MAXWARP - MINWARP) / 2);

// Speed multiplier
inline constexpr int WARPFACTOR = 10; // Warp Factor 10 Mr. Sulu!

// Divides the per-tick z-step so faster timer rates don't make stars
// teleport. Scale this with MINTIMERSPEED: roughly (original_ms / new_ms),
// e.g. 50ms → ~17ms is ~3x faster, so divide each step by 3.
inline constexpr int WARP_STEP_DIVISOR = 3;

// Minimum window timer speed in milliseconds
inline constexpr int MINTIMERSPEED = static_cast<int>(16.7f); // ~60 FPS

// Rand helper macro
#define ZRAND(x) (rand() % (x))

// Color constants
#define RGB_BLACK   RGB(0, 0, 0)
#define RGB_WHITE   RGB(255, 255, 255)
#define RGB_GREY    RGB(128, 128, 128)
#define RGB_RED     RGB(255, 0, 0)
#define RGB_GREEN   RGB(0, 255, 0)
#define RGB_BLUE    RGB(0, 0, 255)
#define RGB_YELLOW  RGB(255, 255, 0)
#define RGB_CYAN    RGB(0, 255, 255)
#define RGB_MAGENTA RGB(255, 0, 255)

#define RGB_STAR RGB_WHITE

#endif // STARFIELD_CONSTANTS_H_
