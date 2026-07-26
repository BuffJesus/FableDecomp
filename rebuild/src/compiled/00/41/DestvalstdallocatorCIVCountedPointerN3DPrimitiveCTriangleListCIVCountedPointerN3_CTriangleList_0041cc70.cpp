struct CountedObj {
    void** vtbl;
    long   refcount;
};
struct CIVCountedPointer {
    CountedObj* p;
};

void __fastcall Dest_val(CIVCountedPointer* self, void* al)
{
    CountedObj* p = self->p;
    if (p != 0) {
        if (--p->refcount == 0) {
            (*(void (__fastcall**)(CountedObj*))(*(void***)p + 1))(p);
        }
        self->p = 0;
    }
}