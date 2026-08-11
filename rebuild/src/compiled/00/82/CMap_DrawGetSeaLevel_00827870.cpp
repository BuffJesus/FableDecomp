// CMap::DrawGetSeaLevel @ 00827870
// retail: fld dword ptr [ecx+0x1d08]; ret 4
// float member getter; ret 4 => one unused stack arg cleaned.

struct CMap {
    char pad[0x1d08];
    float seaLevel; // +0x1d08
};

// this in ECX (self), one unused stack arg (occupies caller cleanup -> ret 4).
float __fastcall DrawGetSeaLevel(CMap* self, int /*edx unused*/, int /*unused stack arg*/)
{
    return self->seaLevel;
}