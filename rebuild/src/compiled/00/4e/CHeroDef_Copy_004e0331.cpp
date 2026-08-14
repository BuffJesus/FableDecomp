#pragma optimize("s",on)
struct CDefClassBase {
    int pad[10];   // 0x00..0x27
    int m28;       // 0x28
    int m2c;       // 0x2c
    void Copy(CDefClassBase const* src);  // thiscall, at 0x431f10
};

struct CHeroDef : public CDefClassBase {
};

void __fastcall CHeroDef_Copy(CHeroDef* self, int edx_ignored, CDefClassBase const* src)
{
    self->CDefClassBase::Copy(src);
    self->m28 = src->m28;
    self->m2c = src->m2c;
}