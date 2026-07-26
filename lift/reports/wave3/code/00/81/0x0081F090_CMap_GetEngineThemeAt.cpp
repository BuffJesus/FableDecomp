unsigned long CMap::GetEngineThemeAt(
    long x,
    long y,
    unsigned long themeValue,
    EThemePaintType paintType) const
{
    struct ThemePaletteEntryOverlay
    {
        unsigned long ThemeId;
        std::byte _04[0x4];
    };
    static_assert(offsetof(ThemePaletteEntryOverlay, ThemeId) == 0x0);
    static_assert(sizeof(ThemePaletteEntryOverlay) == 0x8);

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

    // The supplied PDB layout around 0x1A6C-0x1A7C conflicts with the raw pointer
    // accesses in the binary, so keep this region as an opaque offset-checked overlay.
    struct CMapThemeStorageOverlay
    {
        std::byte _00[0x1A6C];
        ThemePaletteEntryOverlay* ThemePaletteEntriesBegin;
        ThemePaletteEntryOverlay* ThemePaletteEntriesEnd;
        ThemePaletteEntryOverlay* ThemePaletteEntriesCapacityEnd;
        QuarterResThemeGridOverlay* QuarterResThemeGrid;
        GameMapThemeOverlay* GameMapThemeGrid;
    };
    static_assert(offsetof(CMapThemeStorageOverlay, ThemePaletteEntriesBegin) == 0x1A6C);
    static_assert(offsetof(CMapThemeStorageOverlay, ThemePaletteEntriesEnd) == 0x1A70);
    static_assert(offsetof(CMapThemeStorageOverlay, ThemePaletteEntriesCapacityEnd) == 0x1A74);
    static_assert(offsetof(CMapThemeStorageOverlay, QuarterResThemeGrid) == 0x1A78);
    static_assert(offsetof(CMapThemeStorageOverlay, GameMapThemeGrid) == 0x1A7C);

    auto const& storage = reinterpret_cast<CMapThemeStorageOverlay const&>(*this);

    if (paintType == static_cast<EThemePaintType>(0))
    {
        GameMapThemeOverlay* const gameMapThemeGrid = storage.GameMapThemeGrid;
        const unsigned char paletteIndex =
            gameMapThemeGrid->Data[
                ((gameMapThemeGrid->MaxY - y) * (gameMapThemeGrid->Width + 1) + x) * 0xC + 4 +
                themeValue];

        return storage.ThemePaletteEntriesBegin[paletteIndex].ThemeId;
    }

    if (paintType == static_cast<EThemePaintType>(1))
    {
        QuarterResThemeGridOverlay* const quarterResThemeGrid = storage.QuarterResThemeGrid;

        return static_cast<unsigned long>(
            quarterResThemeGrid->Data[
                (((quarterResThemeGrid->Height - ((y + ((y >> 31) & 3)) >> 2)) - 1) *
                     quarterResThemeGrid->Width +
                 ((x + ((x >> 31) & 3)) >> 2)) *
                    7 +
                themeValue]);
    }

    return 0;
}