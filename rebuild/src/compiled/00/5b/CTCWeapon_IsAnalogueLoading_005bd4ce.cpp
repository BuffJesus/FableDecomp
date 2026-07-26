#pragma optimize("g,s",on)
extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)
struct Inner;
struct InnerVtbl {
    void (__fastcall *slot0)(Inner*);
    void (__fastcall *slot1)(Inner*);
};
struct Inner {
    InnerVtbl* vtbl;
    long refcount;
    char pad2[0x58 - 8];
    bool flag;
};
struct CTCWeapon {
    char pad[0x68];
    Inner* inner;
};
bool __fastcall CTCWeapon_IsAnalogueLoading(CTCWeapon* self)
{
    Inner* p = self->inner;
    if (p) ++p->refcount;
    bool v = p->flag;
    _ReadWriteBarrier();
    if (--p->refcount == 0) {
        p->vtbl->slot1(p);
    }
    return v;
}