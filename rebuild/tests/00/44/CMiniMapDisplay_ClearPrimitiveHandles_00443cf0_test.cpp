#include <cstdio>

// Behaviour model of ClearPrimitiveHandles: two vector clears + subobj clear.

struct ElemA { char pad[0xc]; int member_c; };
struct VBase { virtual void slot0(int x) = 0; int other; };
struct SubObj28 { char pad[4]; };

static int g_elemDtors = 0;
static int g_deletes = 0;
static int g_sub28 = 0;
static int g_virt = 0;

void ElemA_dtor(int) { ++g_elemDtors; }
void op_delete(void*) { ++g_deletes; }
void Sub28_clear(SubObj28*) { ++g_sub28; }

struct Concrete : VBase { void slot0(int) { ++g_virt; } };

struct CMiniMapDisplay {
    char pad00[0x1c];
    VBase* vecB_begin;
    VBase* vecB_end;
    char pad24[0x28-0x24];
    SubObj28 sub28;
    char pad2c[0x154-0x2c];
    ElemA* vecA_begin;
    ElemA* vecA_end;
};

void model(CMiniMapDisplay* self)
{
    ElemA* ebx = self->vecA_end;
    ElemA* esi = self->vecA_begin;
    if (esi != ebx) {
        do { ElemA_dtor((int)&esi->member_c); esi = (ElemA*)((char*)esi + 0x10); } while (esi != ebx);
    }
    if (self->vecA_begin) op_delete(self->vecA_begin);
    Sub28_clear(&self->sub28);
    VBase* eb2 = self->vecB_end;
    VBase* es2 = self->vecB_begin;
    if (es2 != eb2) {
        do { es2->slot0(0); es2 = (VBase*)((char*)es2 + 8); } while (es2 != eb2);
    }
    if (self->vecB_begin) op_delete(self->vecB_begin);
}

int main()
{
    CMiniMapDisplay d;
    ElemA av[3];
    Concrete bv[2];
    d.vecA_begin = av; d.vecA_end = av + 3;
    d.vecB_begin = bv; d.vecB_end = bv + 2;

    model(&d);

    // 3 element dtors, 2 virtual clears, 1 sub28 clear, 2 deletes (both vectors non-null)
    if (g_elemDtors == 3 && g_virt == 2 && g_sub28 == 1 && g_deletes == 2) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL e=%d v=%d s=%d d=%d\n", g_elemDtors, g_virt, g_sub28, g_deletes);
    }
    return 0;
}