// Stars logic

#include "stars.h"

LONG nX[MAXSTARS];
LONG nY[MAXSTARS];
LONG nZ[MAXSTARS];

// On-screen size (in pixels) of a star at its closest approach to the camera.
// Stars farther away render smaller, scaling linearly down to MIN_STARSIZE at
// the back of the field. Defaults of 5 / 1 match the original screensaver.
int STARSIZE     = 10;
int MIN_STARSIZE = 1;

COLORREF g_star_color = RGB_STAR; // Default star color is white
COLORREF g_bkg_color  = RGB_SPACE; // Default background color is black

void DrawStars(HWND hWnd, WORD warpfactor) {
  if (hWnd == nullptr) {
    return;
  }
  HDC hDC = GetDC(hWnd);
  if (hDC == nullptr) {
    return;
  }

  // One brush each per frame from current globals — picks up live changes
  // from the configuration dialog without needing invalidation logic.
  HBRUSH hStarBrush = CreateSolidBrush(g_star_color);
  HBRUSH hBkgBrush  = CreateSolidBrush(g_bkg_color);

  int nXTemp, nYTemp, nTemp;
  static WORD wCurrentWarp;
  /* Begin to loop through each star, accelerating so it seems that
     we are traversing the starfield... */
  for (WORD wLoop = 0; wLoop < wDensity; wLoop++) {
    nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                              DIVIDE_SAFE(nZ[wLoop])) +
             wX2Screen;
    nYTemp =
        static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) + wY2Screen;
    nTemp = MIN_STARSIZE + static_cast<int>((SCOPE * WARPFACTOR - nZ[wLoop]) *
                                            (STARSIZE - MIN_STARSIZE) / (SCOPE * WARPFACTOR));
    RECT rErase = {nXTemp, nYTemp, nXTemp + nTemp, nYTemp + nTemp};
    FillRect(hDC, &rErase, hBkgBrush);

    if (wCurrentWarp < warpfactor) {
      wCurrentWarp++;
    } else if (wCurrentWarp > warpfactor) {
      wCurrentWarp--;
    }

    nZ[wLoop] = std::max(0, static_cast<int>(nZ[wLoop] - wCurrentWarp));
    if (!nZ[wLoop]) {
      CreateStar(wLoop);
    }

    nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                              DIVIDE_SAFE(nZ[wLoop])) +
             wX2Screen;
    nYTemp =
        static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) + wY2Screen;
    if ((nXTemp < 0 || nYTemp < 0) ||
        (nXTemp > static_cast<int>(wXScreen) || nYTemp > static_cast<int>(wYScreen))) {
      CreateStar(wLoop);
      nXTemp = static_cast<int>((nX[wLoop] * static_cast<LONG>(SCOPE * WARPFACTOR)) /
                                DIVIDE_SAFE(nZ[wLoop])) +
               wX2Screen;
      nYTemp =
          static_cast<int>((nY[wLoop] * SCOPE * WARPFACTOR) / DIVIDE_SAFE(nZ[wLoop])) + wY2Screen;
    }
    nTemp = MIN_STARSIZE + static_cast<int>((SCOPE * WARPFACTOR - nZ[wLoop]) *
                                            (STARSIZE - MIN_STARSIZE) / (SCOPE * WARPFACTOR));
    RECT rDraw = {nXTemp, nYTemp, nXTemp + nTemp, nYTemp + nTemp};
    FillRect(hDC, &rDraw, hStarBrush);
  }

  DeleteObject(hStarBrush);
  DeleteObject(hBkgBrush);
  ReleaseDC(hWnd, hDC);
}

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
