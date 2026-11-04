#include <algorithm>
#include "fable_surface.h"

void CTexture::CopyFromTexture(const CTexture& source, unsigned long filter)
{
    long levels = std::min(GetNoLevels(), source.GetNoLevels());

    for (long level = 0; level < levels; ++level)
    {
        CSurface sourceSurface = source.GetSurfaceLevel(level);
        CSurface destinationSurface = GetSurfaceLevel(level);
        destinationSurface.CopyFromSurface(0, sourceSurface, 0, filter);
    }
}
