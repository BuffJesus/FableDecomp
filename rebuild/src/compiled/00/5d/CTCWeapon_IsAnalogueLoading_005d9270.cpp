struct RefTarget {
    void** vtbl;
    long refcount;
    char pad[0x5f - 8];
    bool analogueLoading;
};

struct CTCWeapon {
    char pad[0xc];
    RefTarget* m_target;
};

typedef void (__fastcall *ReleaseFn)(RefTarget*);

bool __fastcall CTCWeapon_IsAnalogueLoading(const CTCWeapon* self)
{
    RefTarget* t = self->m_target;
    if (t)
        ++t->refcount;
    bool v = t->analogueLoading;
    long* pr = &t->refcount;
    long rc = *pr - 1;
    *pr = rc;
    if (rc == 0)
        ((ReleaseFn)t->vtbl[1])(t);
    return v;
}