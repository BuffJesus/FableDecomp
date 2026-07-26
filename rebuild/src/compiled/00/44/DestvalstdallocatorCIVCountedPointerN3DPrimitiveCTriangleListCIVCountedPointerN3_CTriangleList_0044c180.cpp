struct CCounted {
    void (__fastcall **vtbl)(CCounted*);
    long refcount;
};

struct CIVCountedPointer {
    CCounted* ptr;
};

void __fastcall Dest_val(CIVCountedPointer* self)
{
    CCounted* c = self->ptr;
    if (c) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);
        }
        self->ptr = 0;
    }
}