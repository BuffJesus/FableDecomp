unsigned char CMap::GetEngineBlendAt(
    long x,
    long y,
    unsigned long blendIndex,
    EThemePaintType paintType) const
{
    struct ThemePaletteEntryListOverlay
    {
        void* Entry0;
        void* Entry1;
    };
    static_assert(offsetof(ThemePaletteEntryListOverlay, Entry0) == 0x0);
    static_assert(offsetof(ThemePaletteEntryListOverlay, Entry1) == 0x4);

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

    auto const* const themePaletteEntryList =
        reinterpret_cast<ThemePaletteEntryListOverlay const*>(std::addressof(this->ThemePaletteEntryList));

    if (paintType == static_cast<EThemePaintType>(0))
    {
        auto const* const gameMapTheme =
            reinterpret_cast<GameMapThemeOverlay const*>(themePaletteEntryList->Entry1);

        return gameMapTheme->Data[
            ((gameMapTheme->MaxY - y) * (gameMapTheme->Width + 1) + x) * 0xC + 7 + blendIndex];
    }

    if (paintType == static_cast<EThemePaintType>(1))
    {
        auto const* const quarterResTheme =
            reinterpret_cast<QuarterResThemeGridOverlay const*>(themePaletteEntryList->Entry0);

        return reinterpret_cast<CGameMapCell*>(
                   quarterResTheme->Data +
                   ((((quarterResTheme->Height - ((y + ((y >> 31) & 3)) >> 2)) - 1) *
                         quarterResTheme->Width +
                     ((x + ((x >> 31) & 3)) >> 2)) *
                    7))
            ->GetAtmosBlend(blendIndex);
    }

    return 0;
}