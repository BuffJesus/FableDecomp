struct SubObj {
    char pad[0x6c];
    unsigned char flags; // +0x6c
};

struct CTCPhysicsBase;

typedef int (__fastcall *SlotFn)(CTCPhysicsBase* self);

struct CTCPhysicsBaseVtbl {
    char pad[0x174];
    SlotFn slot174; // +0x174
};

struct CTCPhysicsBase {
    CTCPhysicsBaseVtbl* vtbl; // +0x00
    SubObj* sub;              // +0x04
};

bool __fastcall IsPhysicsMeshTypeCylinder(CTCPhysicsBase* self)
{
    if (self->vtbl->slot174(self) == 0 && (self->sub->flags & 8))
        return true;
    return false;
}