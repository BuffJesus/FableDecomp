// Byte-exact reconstruction of CGraphicDataBank::ChangeScene @ 0x00c22a00
// this-call (self in ecx) modeled as __fastcall free fn (leading ptr -> ecx).

struct CGraphicDataBank {
    void* vtbl;      // +0x00
    char  pad[0x10 - 0x04];
    void* p10;       // +0x10
    void* p14;       // +0x14
};

// cdecl free function target at 0xbfeb1c (operator delete / free-like).
extern "C" void __cdecl FreeThing(void* p);

// vtable address baked as a constant.
#define VTBL_ADDR ((void*)0x12b3820)

void __fastcall ChangeScene(CGraphicDataBank* self)
{
    self->vtbl = VTBL_ADDR;
    FreeThing(self->p14);
    FreeThing(self->p10);
}