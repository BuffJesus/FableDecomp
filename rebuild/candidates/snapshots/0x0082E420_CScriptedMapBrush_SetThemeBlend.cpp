void CScriptedMapBrush::SetThemeBlend(long x, long y, unsigned char blend, unsigned long layer)
{
    struct BrushOverlay
    {
        unsigned char Padding0[0x08];
        unsigned char* CellsData;
        unsigned char Padding1[0x38 - 0x0C];
        int MinX;
        int MinY;
        int MaxX;
        int MaxY;
    };
    static_assert(offsetof(BrushOverlay, CellsData) == 0x08);
    static_assert(offsetof(BrushOverlay, MinX) == 0x38);
    static_assert(offsetof(BrushOverlay, MinY) == 0x3C);
    static_assert(offsetof(BrushOverlay, MaxX) == 0x40);
    static_assert(offsetof(BrushOverlay, MaxY) == 0x44);

    auto* const self = reinterpret_cast<BrushOverlay*>(this);

    if ((((self->MinX <= x) && (x < self->MaxX)) && (self->MinY <= y)) && (y < self->MaxY))
    {
        self->CellsData[((((y - self->MinY) * (self->MaxX - self->MinX)) - self->MinX) + x) * 0x14 + 0x10 + layer] = blend;
    }
}