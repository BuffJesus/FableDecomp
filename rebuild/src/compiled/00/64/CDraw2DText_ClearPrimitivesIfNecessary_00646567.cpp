#pragma optimize("s",on)
// CDraw2DText::ClearPrimitivesIfNecessary @ 0x00646567
struct Sub28 { char pad[4]; };

struct CDraw2DText {
    char pad0[0x18];
    unsigned char f18;   // +0x18
    char pad19[0x28-0x19];
    Sub28 f28;           // +0x28
};

// helper at 0x440770: __fastcall on (this+0x28)
extern void __fastcall helper_440770(Sub28* self);
// tail target at 0x644627: __fastcall void on this
extern void __fastcall base_644627(CDraw2DText* self);

void __fastcall CDraw2DText_ClearPrimitivesIfNecessary(CDraw2DText* self)
{
    if (self->f18 == 0)
        helper_440770(&self->f28);
    base_644627(self);
}