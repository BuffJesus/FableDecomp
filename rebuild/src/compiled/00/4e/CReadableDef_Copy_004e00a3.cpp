// CReadableDef::Copy @ 004e00a3
struct CDefClassBase { char pad[0x40]; };

struct CReadableBase {
    void Copy(CDefClassBase const* src); // __fastcall base at 0x431f10
};

struct CReadableDef : CReadableBase {
    unsigned char field25pad[0x25];
    unsigned char field25; // 0x25
    void Copy(CDefClassBase const* src);
};

void __fastcall CReadableDef_Copy(CReadableDef* self, int /*edx*/, CDefClassBase const* src)
{
    self->CReadableBase::Copy(src);
    self->field25 = ((const CReadableDef*)src)->field25;
}