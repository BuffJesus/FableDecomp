// Manually reviewed: VC7.1 output is relocation-masked byte-identical to the
// 96-byte retail body at 0x0063AAB0, including unsigned-char zero extension.
unsigned long CWorldMap::GetThemeBlendAtBlock(
    const C2DCoordI& worldMapCoord,
    unsigned long themeId,
    EThemePaintType paintType) const
{
    const long mapNumber = GetMapNumberAt(worldMapCoord);
    if (mapNumber != 0)
    {
        const C2DCoordI localCoord =
            ConvertWorldMapCoordsToLocalCoords(mapNumber, worldMapCoord);
        return PeekMap(mapNumber).GetEngineBlendAt(
            localCoord.x,
            localCoord.y,
            themeId,
            paintType);
    }
    return 0;
}
