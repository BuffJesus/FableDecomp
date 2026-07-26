#pragma optimize("s",on)
struct CRefCounted {
    void** vtbl;   // +0x0
    long   refs;   // +0x4
};

struct CIVCountedPointer {
    CRefCounted* p;  // +0x0
};

typedef void (__fastcall *DtorFn)(CRefCounted*);

void __fastcall CIVCountedPointer_Dest_val(CIVCountedPointer* self)
{
    CRefCounted* obj = self->p;
    if (obj) {
        if (--obj->refs == 0) {
            (*(DtorFn*)((char*)obj->vtbl + 4))(obj);
        }
        self->p = 0;
    }
}