#pragma optimize("s",on)
// Byte-exact reconstruction of the CIVCountedPointer<CTriangleList> destroy body
// inlined as _Dest_val<...> at retail 0x005920e1.
struct RefCounted { void** vtable; int refCount; };
struct CIVCountedPointer_CTriangleList___ { RefCounted* ptr; };
void __fastcall Dest_val_CIVCountedPointer_CTriangleList___(
        CIVCountedPointer_CTriangleList___* self)
{
    RefCounted* rc = self->ptr;
    if (rc != 0) {
        if (--rc->refCount == 0) {
            typedef void (__fastcall *pfn)(RefCounted*);
            (*(pfn*)((char*)rc->vtable + 4))(rc);
        }
        self->ptr = 0;
    }
}