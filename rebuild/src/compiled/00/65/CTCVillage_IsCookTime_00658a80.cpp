#pragma optimize("s",on)
struct RefObj;
typedef void (__fastcall *Dtor)(RefObj*);
struct RefObj {
    Dtor* vtbl;
    long ref;
    char pad[0x50 - 8];
    void* member50;
};

extern bool __stdcall CheckCookTime(void* subobj, void* arg);

struct CTCVillage {
    char pad[0x94];
    RefObj* smartptr;
};

bool __fastcall CTCVillage_IsCookTime(CTCVillage* self)
{
    RefObj** pp = &self->smartptr;
    RefObj* a = *pp;
    if (a) a->ref++;
    RefObj* b = *pp;
    if (b) b->ref++;

    bool result = CheckCookTime((char*)b + 0x44, a->member50);

    if (b) {
        if (--b->ref == 0) {
            b->vtbl[1](b);
        }
    }
    if (a) {
        if (--a->ref == 0) {
            a->vtbl[1](a);
        }
    }
    return result;
}