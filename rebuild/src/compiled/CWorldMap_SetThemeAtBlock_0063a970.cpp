#include "candidates/world_map_theme.h"

void CWorldMap::SetThemeAtBlock(
    const C2DCoordI& worldMapCoord,
    unsigned long themeId,
    unsigned long themeValue,
    EThemePaintType paintType)
{
    const long mapNumber = GetMapNumberAt(worldMapCoord);
    if (mapNumber != 0 && IsPosChangeable(worldMapCoord))
    {
        const C2DCoordI localCoord =
            ConvertWorldMapCoordsToLocalCoords(mapNumber, worldMapCoord);
        GetMap(mapNumber).SetEngineThemeAt(
            localCoord.x,
            localCoord.y,
            themeId,
            themeValue,
            paintType);
    }
}
