#pragma optimize("t", on)

struct Handle { unsigned char pad[0x14]; };
struct HandleVec {
    void*   proxy;    // +0x00
    Handle* first;    // +0x04
    Handle* last;     // +0x08
    void*   end;      // +0x0C
};
struct CMiniMapDisplay {
    HandleVec vecB;   // +0x00
    HandleVec vecA;   // +0x10
};

extern void __fastcall HandleDtor(void* self);
extern void __fastcall ObjectDtor(CMiniMapDisplay* self);
extern void __cdecl EngineFree(void* p);

static void ClearVec(HandleVec* v)
{
    Handle* last = v->last;
    for (Handle* p = v->first; p != last; ++p)
        HandleDtor(p);
    if (v->first)
        EngineFree(v->first);
    HandleDtor(v);
}

void __fastcall CMiniMapDisplay_ClearPrimitiveHandles(CMiniMapDisplay* self)
{
    ClearVec(&self->vecA);

    Handle* last2 = self->vecB.last;
    for (Handle* p = self->vecB.first; p != last2; ++p)
        HandleDtor(p);
    if (self->vecB.first)
        EngineFree(self->vecB.first);

    ObjectDtor(self);
}