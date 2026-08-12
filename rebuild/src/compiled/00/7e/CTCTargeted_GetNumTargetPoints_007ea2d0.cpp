// CTCTargeted::GetNumTargetPoints @ 0x007ea2d0
// long __fastcall GetNumTargetPoints(CTCTargeted* this)

struct Inner {
    char pad[0x64];
    Inner* sub; // +0x64
    // external __fastcall method at 0x4bde90: (this, void*, float) -> long
    long Query(void* p, float f);
};

struct CTCTargeted {
    void* v;      // +0
    Inner* obj;   // +4
};

long __fastcall CTCTargeted_GetNumTargetPoints(CTCTargeted* self)
{
    Inner* o = self->obj->sub;
    if (o)
        return o->Query((void*)0x13bac50, 1.0f);
    return 0;
}