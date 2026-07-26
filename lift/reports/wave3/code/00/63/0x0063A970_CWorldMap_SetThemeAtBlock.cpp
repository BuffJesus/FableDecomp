// Manually reviewed: VC7.1 output is relocation-masked byte-identical to the
// 99-byte retail body at 0x0063A970.
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
