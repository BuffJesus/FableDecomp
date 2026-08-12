#pragma optimize("s",on)
// Retail 0x00485015
// std::_Dest_val<allocator<CIVCountedPointer<...>>, CIVCountedPointer<...>>
// Destroys a CIVCountedPointer: decrement the referenced object's refcount;
// when it hits zero, virtual-release it; then null the held pointer.

struct CountedObject {
    virtual void v0();   // slot 0
    virtual void v1();   // slot 1 -> call [eax+4]
    // refcount lives at +4; the vtable ptr occupies +0
    int refcount;        // +4
};

struct CIVCountedPointer {
    CountedObject* p;    // +0
};

// this-in-ecx: model as a __fastcall taking self in ecx.
void __fastcall CIVCountedPointer_Dest_val(CIVCountedPointer* self)
{
    CountedObject* c = self->p;
    if (c) {
        if (--c->refcount == 0) {
            c->v1();
        }
        self->p = 0;
    }
}