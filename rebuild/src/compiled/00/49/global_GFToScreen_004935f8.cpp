// 0x004935f8 GFToScreen: __fastcall copy of a 16-byte (4-dword) struct.
struct GFV16 { long m0, m1, m2, m3; };

struct GFV16* __fastcall GFToScreen(struct GFV16* self, int /*edx*/, const struct GFV16* src)
{
    self->m0 = src->m0;
    self->m1 = src->m1;
    self->m2 = src->m2;
    self->m3 = src->m3;
    return self;
}