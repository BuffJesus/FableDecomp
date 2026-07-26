struct CCountedObj {
    void (__fastcall** vtbl)(CCountedObj*);
    long refcount;
};

struct CIVCountedPointer {
    CCountedObj* p;
};

void __fastcall Dest_val(CIVCountedPointer* self, void* unused)
{
    CCountedObj* c = self->p;
    if (c != 0) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);
        }
        self->p = 0;
    }
}