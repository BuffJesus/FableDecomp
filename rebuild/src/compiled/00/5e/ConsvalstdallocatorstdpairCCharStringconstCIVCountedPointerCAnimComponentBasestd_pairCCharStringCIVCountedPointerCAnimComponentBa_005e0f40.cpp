// Byte-exact reconstruction of _Cons_val<...> at 0x005e0f40
// Constructs a std::pair<CCharString, CIVCountedPointer<CAnimComponentBase> > at *self
// from source *src. self in ecx, src in edx (__fastcall).

struct RefCounted {
    void* vfp;
    int   refcount; // at +4
};

struct CIVCountedPointer {
    RefCounted* p; // at +0 of this member; but in the pair it's at +4
};

// CCharString copy-ctor: this in ecx, const CCharString& src pushed (thiscall, callee-cleans).
// Modeled as a member so VC7.1 emits a real thiscall (this in ecx, one stack arg, no edx).
struct CCharString {
    char* buf;
    void CopyCtor(const CCharString& src); // extern; body elsewhere
};

// The pair: +0 CCharString (first), +4 CIVCountedPointer (second holds RefCounted*)
struct Pair {
    CCharString      first;  // +0
    RefCounted*      second; // +4
};

void __fastcall Cons_val(Pair* self, Pair* src)
{
    if (self)
    {
        self->first.CopyCtor(src->first);
        RefCounted* rc = src->second;
        self->second = rc;
        if (rc)
            rc->refcount++;
    }
}