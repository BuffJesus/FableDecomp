struct C2DBoxI { int x0, y0, x1, y1; };

struct CWorldMap {
    char pad[0x5c];
    C2DBoxI dimensions;
};

C2DBoxI const & __fastcall GetDimensions(CWorldMap *self)
{
    return self->dimensions;
}