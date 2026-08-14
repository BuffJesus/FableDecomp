#pragma optimize("s",on)
// CCarriedReadableDef::Copy  0x004e157c
struct CDefClassBase { int dummy; };

struct CCarriedReadableDef {
    char pad[0x28];
    int  field28;  // +0x28
    int  field2c;  // +0x2c
};

// base Copy(other) — stdcall callee taking other on the stack (cleans its own arg)
extern "C" void __stdcall BaseCopy_431f10(CDefClassBase const* other);

void __fastcall CCarriedReadableDef_Copy(CCarriedReadableDef* self, void* /*edx*/, CDefClassBase const* other)
{
    BaseCopy_431f10(other);
    const CCarriedReadableDef* o = (const CCarriedReadableDef*)other;
    self->field28 = o->field28;
    self->field2c = o->field2c;
}