#pragma optimize("s",on)
struct RefObj {
    void** vtbl;
    int refcount;
    char pad[0x54 - 8];
    int field54; // used as &field54 arg
    char pad2[0x60 - 0x58];
    int field60; // used as value arg
};

struct RefPtr {
    RefObj* p;
};

struct CTCVillage {
    char pad[0x94];
    RefPtr ptr;
};

extern bool __stdcall DoCookCheck(int* a, int b);

bool __fastcall CTCVillage_IsCookTime(CTCVillage* self)
{
    RefPtr* slot = &self->ptr;
    RefObj* a = slot->p;
    if (a) a->refcount++;
    RefObj* b = slot->p;
    if (b) b->refcount++;

    bool result = DoCookCheck(&b->field54, a->field60);

    if (b) {
        if (--b->refcount == 0)
            ((void(__fastcall*)(RefObj*))b->vtbl[1])(b);
    }
    if (a) {
        if (--a->refcount == 0)
            ((void(__fastcall*)(RefObj*))a->vtbl[1])(a);
    }
    return result;
}