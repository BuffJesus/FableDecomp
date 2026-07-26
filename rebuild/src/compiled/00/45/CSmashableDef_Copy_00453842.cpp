#pragma optimize("s",on)
struct CBaseDef {
    void Copy(const CBaseDef* src);
};

struct CSmashableDef : public CBaseDef {
    char pad[0x25];
    unsigned char b25;
    char pad2[0x28 - 0x26];
    int d28;
    int d2c;
};

void __fastcall CSmashableDef_Copy(CSmashableDef* self, void* edx, const CSmashableDef* src)
{
    self->Copy(src);
    self->b25 = src->b25;
    self->d28 = src->d28;
    self->d2c = src->d2c;
}