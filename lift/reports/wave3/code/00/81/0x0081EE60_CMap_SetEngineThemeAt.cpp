void CMap::SetEngineThemeAt(
    long x,
    long y,
    unsigned long themeId,
    unsigned long themeValue,
    EThemePaintType paintType)
{
    struct QuarterResThemeGridOverlay
    {
        std::byte _00[0x4];
        unsigned char* Data;
        std::byte _08[0x14];
        long Width;
        long Height;
    };
    static_assert(offsetof(QuarterResThemeGridOverlay, Data) == 0x4);
    static_assert(offsetof(QuarterResThemeGridOverlay, Width) == 0x1C);
    static_assert(offsetof(QuarterResThemeGridOverlay, Height) == 0x20);

    struct GameMapThemeOverlay
    {
        std::byte _00[0x10];
        unsigned char* Data;
        std::byte _14[0x1F0];
        long Width;
        long MaxY;
    };
    static_assert(offsetof(GameMapThemeOverlay, Data) == 0x10);
    static_assert(offsetof(GameMapThemeOverlay, Width) == 0x204);
    static_assert(offsetof(GameMapThemeOverlay, MaxY) == 0x208);

    // The supplied PDB field list around 0x1A78 conflicts with the raw pointer
    // usage in this body, so keep that region as an opaque offset-checked overlay.
    struct CMapThemeStorageOverlay
    {
        std::byte _00[0x1A78];
        QuarterResThemeGridOverlay* QuarterResThemeGrid;
        GameMapThemeOverlay* GameMapThemeGrid;
    };
    static_assert(offsetof(CMapThemeStorageOverlay, QuarterResThemeGrid) == 0x1A78);
    static_assert(offsetof(CMapThemeStorageOverlay, GameMapThemeGrid) == 0x1A7C);

    auto const& storage = reinterpret_cast<CMapThemeStorageOverlay const&>(*this);

    if (paintType != static_cast<EThemePaintType>(0))
    {
        if (paintType == static_cast<EThemePaintType>(1))
        {
            QuarterResThemeGridOverlay* const quarterResThemeGrid = storage.QuarterResThemeGrid;
            const long quarterResX = (x + ((x >> 31) & 3)) >> 2;
            const long quarterResY = (y + ((y >> 31) & 3)) >> 2;

            quarterResThemeGrid->Data[
                (((quarterResThemeGrid->Height - quarterResY) - 1) * quarterResThemeGrid->Width +
                 quarterResX) *
                    7 +
                themeValue] = static_cast<unsigned char>(themeId);
        }
        return;
    }

    GameMapThemeOverlay* const gameMapThemeGrid = storage.GameMapThemeGrid;
    if (themeId != 0)
    {
        const unsigned char paletteIndex = AddThemeDefIndexToPalette(themeId);
        gameMapThemeGrid->Data[
            ((gameMapThemeGrid->MaxY - y) * (gameMapThemeGrid->Width + 1) + x) * 0xC + 4 +
            themeValue] = paletteIndex;
        return;
    }

    gameMapThemeGrid->Data[
        ((gameMapThemeGrid->MaxY - y) * (gameMapThemeGrid->Width + 1) + x) * 0xC + 4 +
        themeValue] = 0;
}