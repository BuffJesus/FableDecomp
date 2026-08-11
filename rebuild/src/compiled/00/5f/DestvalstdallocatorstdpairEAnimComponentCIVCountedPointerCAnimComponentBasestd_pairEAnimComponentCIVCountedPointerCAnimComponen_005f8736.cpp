#pragma optimize("s",on)
// _Dest_val for std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>
// Body only touches the counted pointer at pair+4: decref, release on zero, null out.
// __fastcall: ecx = self (the pair), edx = unused allocator/formal. No stack params (ret 0).

struct Counted {
    virtual void v0();
    virtual void v1();   // slot 1 = call [vtbl+4]  (release/destroy)
    int refcount;        // +4
};

struct AnimPair {
    int first;           // +0  EAnimComponent
    Counted* ptr;        // +4  CIVCountedPointer inner pointer
};

void __fastcall _Dest_val(AnimPair* self, int /*edx*/)
{
    Counted* c = self->ptr;
    if (c != 0) {
        if (--c->refcount == 0) {
            c->v1();
        }
        self->ptr = 0;
    }
}