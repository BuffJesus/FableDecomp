#include "candidates/world_map_theme.h"

void CWorldMap::SetThemeBlendAtBlock(
    const C2DCoordI& worldMapCoord,
    unsigned char blendValue,
    unsigned long themeId,
    EThemePaintType paintType)
{
    const long mapNumber = GetMapNumberAt(worldMapCoord);
    if (mapNumber != 0 && IsPosChangeable(worldMapCoord))
    {
        const C2DCoordI localCoord =
            ConvertWorldMapCoordsToLocalCoords(mapNumber, worldMapCoord);
        GetMap(mapNumber).SetEngineBlendAt(
            localCoord.x,
            localCoord.y,
            blendValue,
            themeId,
            paintType);
    }
}
