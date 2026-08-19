#pragma optimize("s",on)
// CCardDef::Copy  0x004ec0f0
// __fastcall void CCardDef::Copy(CCardDef* this, CDefClassBase const* src)
// Modeled as __fastcall with an edx dummy so `src` lands on the stack (true __fastcall shape).

struct CDefClassBase;

struct CCardDef {
    char pad[0x3c];
    unsigned int f3c;   // +0x3c
    unsigned int f40;   // +0x40
};

// base-class Copy (non-virtual direct call @ 0x431ed0), __fastcall(this, src)
extern "C" void __fastcall BaseCopy(CCardDef* self, int edx_, CDefClassBase const* src);

void __fastcall CCardDef_Copy(CCardDef* self, int edx_, CDefClassBase const* src)
{
    BaseCopy(self, edx_, src);
    self->f3c = ((CCardDef const*)src)->f3c;
    self->f40 = ((CCardDef const*)src)->f40;
}