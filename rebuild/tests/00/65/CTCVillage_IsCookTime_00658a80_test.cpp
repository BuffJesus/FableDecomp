#include <cstdio>

struct RefObj;
typedef void (__fastcall *Dtor)(RefObj*);
struct RefObj {
    Dtor* vtbl;
    long ref;
    char pad[0x50 - 8];
    void* member50;
};

static void* g_subobj_seen = 0;
static int g_arg2_seen = 0;

bool __stdcall CheckCookTime(void* subobj, void* arg) {
    g_subobj_seen = subobj;
    g_arg2_seen = (arg != 0) ? 1 : 0;
    return true;
}

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

int main() {
    RefObj obj;
    obj.vtbl = 0;
    obj.ref = 5;
    obj.member50 = (void*)0x1234;

    CTCVillage v;
    v.smartptr = &obj;

    bool r = CTCVillage_IsCookTime(&v);

    if (r == true && obj.ref == 5 && g_subobj_seen == ((char*)&obj + 0x44) && g_arg2_seen == 1) {
        std::printf("CTCVillage_00658a80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d ref=%ld\n", (int)r, obj.ref);
    return 1;
}