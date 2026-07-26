#include "candidates/world_map_theme.h"

unsigned long CWorldMap::GetThemeAtBlock(
    const C2DCoordI& worldMapCoord,
    unsigned long themeId,
    EThemePaintType paintType) const
{
    const long mapNumber = GetMapNumberAt(worldMapCoord);
    if (mapNumber != 0)
    {
        const C2DCoordI localCoord =
            ConvertWorldMapCoordsToLocalCoords(mapNumber, worldMapCoord);
        return PeekMap(mapNumber).GetEngineThemeAt(
            localCoord.x,
            localCoord.y,
            themeId,
            paintType);
    }
    return 0;
}
