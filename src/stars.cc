// Stars logic

#include "stars.h"

LONG nX[MAXSTARS];
LONG nY[MAXSTARS];
LONG nZ[MAXSTARS];

void CreateStar(WORD wIndex) {
  nX[wIndex] =
      wXScreen ? static_cast<LONG>(static_cast<int>(ZRAND(wXScreen)) - static_cast<int>(wX2Screen))
               : 0;
  nY[wIndex] =
      wXScreen ? static_cast<LONG>(static_cast<int>(ZRAND(wYScreen)) - static_cast<int>(wY2Screen))
               : 0;
  nZ[wIndex] = SCOPE * WARPFACTOR;
}

void ssrand(DWORD dwSeed) {
  ::srand(static_cast<unsigned int>(dwSeed));
}
