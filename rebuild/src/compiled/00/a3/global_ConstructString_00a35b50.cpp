// ConstructString @ 00a35b50
struct Inner {
    bool Build(void* a, long b, int zero);   // thiscall member: ecx=this, stack args a,b,zero
};

struct CWideString {
    char pad0[8];
    Inner* field8;   // +8
};

bool __fastcall ConstructString(CWideString* self, int /*edx*/, void* a, long b)
{
    return self->field8->Build(a, b, 0);
}