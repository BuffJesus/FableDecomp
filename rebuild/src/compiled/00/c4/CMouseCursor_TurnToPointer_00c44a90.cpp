struct Inner0 { char pad[0x160]; float scale; };
struct Inner4 { char pad4[4]; };
struct CMouseCursor { Inner0* f0; Inner4* f4; };
extern "C" float __fastcall Inner4_GetVal(Inner4* self);
float __fastcall CMouseCursor_TurnToPointer(CMouseCursor* self)
{
    float v = Inner4_GetVal(self->f4);
    return v * self->f0->scale;
}