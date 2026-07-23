struct Counted {
    void (__fastcall **vtbl)(Counted*);
    long ref;
};
struct Ptr {
    Counted* p;
};

void __fastcall Dest_val(Ptr* self)
{
    Counted* c = self->p;
    if (c != 0) {
        if (--c->ref == 0) {
            c->vtbl[1](c);
        }
        self->p = 0;
    }
}