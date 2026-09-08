#include <math.h>
struct C2DCoordI { long X; long Y; };
float __fastcall GFGet2DDistance(const C2DCoordI* first, const C2DCoordI* second) {
    const long deltaX = first->X - second->X;
    const long deltaY = first->Y - second->Y;
    return sqrtf(static_cast<float>(deltaX * deltaX + deltaY * deltaY));
}