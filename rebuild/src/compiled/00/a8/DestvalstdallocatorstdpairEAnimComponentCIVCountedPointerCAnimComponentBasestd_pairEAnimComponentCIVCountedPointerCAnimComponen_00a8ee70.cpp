// Counted object: vtable ptr at +0, refcount at +4.
struct CAnimComponentBase {
    virtual void v0();      // slot 0
    virtual void release(); // slot 1 -> call [eax+4]
};

// The pair object; the counted pointer lives at +0x2c.
struct PairHolder {
    char pad[0x2c];
    CAnimComponentBase* ptr; // +0x2c
};

// __fastcall so ecx = self (the pair holder). edx is the unused allocator arg
// slot in this decoration; we ignore it. Leaf name must contain _Dest_val.
void __fastcall _Dest_val_release(PairHolder* self, int /*edx*/)
{
    CAnimComponentBase* p = self->ptr;
    if (p) {
        if (--(*(int*)((char*)p + 4)) == 0) {
            p->release();
        }
        self->ptr = 0;
    }
}