// Donor-PDB identity correction: this is SetThemeBlendAtBlock, whose first
// paint value is unsigned char. VC7.1 output is relocation-masked
// byte-identical to the 99-byte retail body at 0x0063A9E0.
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
