// Byte-exact reconstruction of CCreatureActionHeroJump::FrameUpdate 0x0085bad0

struct SubObj8;

// method at 0xa01b50 called with ecx = &self at +8 (subobject)
void __fastcall Sub8_Method(SubObj8* self);

struct CCreatureActionHeroJump {
    int   m00;      // +0x00
    int   m04;      // +0x04
    int   m08;      // +0x08  (subobject start)
    int   m0c;      // +0x0c
    int   m10;      // +0x10
    int   m14;      // +0x14
    int   m18;      // +0x18
    int   m1c;      // +0x1c
};

// tail-called method at 0x859d80 (ecx = self)
void __fastcall Base_FrameStep(CCreatureActionHeroJump* self);

void __fastcall FrameUpdate(CCreatureActionHeroJump* self)
{
    int v = self->m1c + 7;
    if (v >= self->m10) {
        Sub8_Method((SubObj8*)&self->m08);
    }
    Base_FrameStep(self);
}