// Byte-exact reconstruction of CTCWeapon::GetAnimationSpeedValue @ 0x006625c0
// Pattern: scoped intrusive-refcount hold on a smart-pointer member (+0x70),
// read a float member (+0x60), release (virtual slot 1 on last ref), return float.

struct Referenced {
    void** vtbl;      // +0x00
    int    refcount;  // +0x04
    char   pad[0x58]; // +0x08 .. +0x5F
    float  animSpeed; // +0x60
};

struct CTCWeapon {
    char       pad[0x70];  // +0x00 .. +0x6F
    Referenced* target;    // +0x70
};

typedef void (__fastcall *rel_t)(Referenced*);

float __fastcall CTCWeapon_GetAnimationSpeedValue(CTCWeapon* self)
{
    Referenced* p = self->target;
    if (p != 0)
        p->refcount++;
    float v = p->animSpeed;
    if (--p->refcount == 0)
        ((rel_t)p->vtbl[1])(p);
    return v;
}