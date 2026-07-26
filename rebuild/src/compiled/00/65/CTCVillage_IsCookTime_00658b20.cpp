#pragma optimize("s",on)
struct RCObj {
    void** vtbl;
    int    refcnt;
    char   pad[0x68];
    int    field70;
};

extern bool __stdcall CookTest(void* a, int b);

struct CTCVillage {
    char pad[0x94];
    RCObj* handle;
};

bool __fastcall CTCVillage_IsCookTime(CTCVillage* self)
{
    RCObj** slot = &self->handle;
    RCObj* a = *slot;
    if (a) a->refcnt++;
    RCObj* b = *slot;
    if (b) b->refcnt++;

    bool r = CookTest((char*)b + 0x64, a->field70);

    if (b) {
        if (--b->refcnt == 0)
            ((void(__fastcall*)(RCObj*))b->vtbl[1])(b);
    }
    if (a) {
        if (--a->refcnt == 0)
            ((void(__fastcall*)(RCObj*))a->vtbl[1])(a);
    }
    return r;
}