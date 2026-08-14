// Byte-exact reconstruction of CAIStateGroupBase::PeekThingSearchTools @ 0x00c38550
struct CThingSearchTools;

struct Inner3 {
    char pad[0x70];
    float rate; // +0x70
};
struct Inner2 {
    char pad[4];
    Inner3* p4; // +4
};
struct Inner1 {
    char pad[4];
    Inner2* p4; // +4
};

struct Sub;
// method at self+0xc, __fastcall, float arg -> CThingSearchTools*
extern "C" CThingSearchTools* __fastcall SubPeek(Sub* self, float scaled);

struct CAIStateGroupBase {
    char pad0[4];
    Inner1* f4;     // +4
    char pad8[4];   // +8
    char sub[4];    // +0xc subobject
};

extern const float g_const; // 0x1252370

CThingSearchTools* __fastcall PeekThingSearchTools(CAIStateGroupBase* self)
{
    return SubPeek((Sub*)((char*)self + 0xc), g_const / self->f4->p4->p4->rate);
}