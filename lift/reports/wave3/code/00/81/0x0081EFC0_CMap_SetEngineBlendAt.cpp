void CMap::SetEngineBlendAt(
    long x,
    long y,
    unsigned char blendValue,
    unsigned long blendIndex,
    EThemePaintType paintType)
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

    auto* const themePaletteEntryList =
        reinterpret_cast<ThemePaletteEntryListOverlay*>(std::addressof(this->ThemePaletteEntryList));

    if (paintType == static_cast<EThemePaintType>(0))
    {
        auto* const gameMapTheme =
            reinterpret_cast<GameMapThemeOverlay*>(themePaletteEntryList->Entry1);

        gameMapTheme->Data[
            ((gameMapTheme->MaxY - y) * (gameMapTheme->Width + 1) + x) * 0xC + 7 + blendIndex] =
            blendValue;
        return;
    }

    if (paintType == static_cast<EThemePaintType>(1))
    {
        auto* const quarterResTheme =
            reinterpret_cast<QuarterResThemeGridOverlay*>(themePaletteEntryList->Entry0);

        quarterResTheme->Data[
            (((quarterResTheme->Height - ((y + ((y >> 31) & 3)) >> 2)) - 1) * quarterResTheme->Width +
             ((x + ((x >> 31) & 3)) >> 2)) *
                7 +
            3 + blendIndex] = blendValue;
    }
}