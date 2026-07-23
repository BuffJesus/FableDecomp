struct CCounted {
    void* vtbl;      // +0x0
    int   refcount;  // +0x4
};

struct CCountedPtr {
    CCounted* p;     // +0x0
};

// _Dest_val<allocator<CIVCountedPointer<CTriangleList> >, CIVCountedPointer<CTriangleList> >
// __fastcall: this in ecx (self = CCountedPtr*), second param = allocator (unused)
void __fastcall _Dest_val(CCountedPtr* self, void* alloc)
{
    CCounted* obj = self->p;
    if (obj) {
        if (--obj->refcount == 0) {
            (*(void (__fastcall**)(CCounted*))((char*)obj->vtbl + 4))(obj);
        }
        self->p = 0;
    }
}