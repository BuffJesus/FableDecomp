#include <cstdio>

struct RefObj {
    void** vtbl;
    int refcount;
    char pad[0x54 - 8];
    int field54;
    char pad2[0x60 - 0x58];
    int field60;
};

struct RefPtr { RefObj* p; };

struct CTCVillage {
    char pad[0x94];
    RefPtr ptr;
};

static int g_last_a = -1;
static int g_last_b = -1;
static int g_disposed = 0;

bool __stdcall DoCookCheck(int* a, int b) {
    g_last_a = *a; // reads field54 of esi/b
    g_last_b = b;  // field60 of edi/a
    return true;
}

void __fastcall Dispose(RefObj* o) { g_disposed++; }

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

int main() {
    void* vt[2];
    vt[1] = (void*)&Dispose;

    RefObj obj;
    obj.vtbl = (void**)vt;
    obj.refcount = 5; // never hits zero
    obj.field54 = 111;
    obj.field60 = 222;

    CTCVillage v;
    v.ptr.p = &obj;

    bool r = CTCVillage_IsCookTime(&v);
    // refcount started 5, +2 -2 = 5, no dispose
    if (r && g_last_a == 111 && g_last_b == 222 && obj.refcount == 5 && g_disposed == 0) {
        std::printf("CTCVillage_00658a35_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d a=%d b=%d rc=%d disp=%d\n", r, g_last_a, g_last_b, obj.refcount, g_disposed);
    return 1;
}