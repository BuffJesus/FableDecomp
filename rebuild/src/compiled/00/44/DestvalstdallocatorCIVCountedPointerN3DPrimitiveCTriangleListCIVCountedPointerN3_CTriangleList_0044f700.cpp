struct RefCounted {
    void** vtbl;
    int refcount;
};

struct CIVCountedPointer {
    RefCounted* p;
};

void __fastcall Dest_val(CIVCountedPointer* self, void* alloc) {
    RefCounted* p = self->p;
    if (p) {
        if (--p->refcount == 0) {
            (*(void (__fastcall**)(RefCounted*))((char*)p->vtbl + 4))(p);
        }
        self->p = 0;
    }
}