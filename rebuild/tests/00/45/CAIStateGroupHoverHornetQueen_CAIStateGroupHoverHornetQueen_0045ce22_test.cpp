#include <stdio.h>

static int g_order = 0;
static int g_a48 = 0, g_b28 = 0, g_base = 0, g_vtbl_when = 0;
static void* g_vtbl_set = 0;

static int VTBL_CONST = 0;

struct MemberA48 { void* dummy; };
struct MemberB28 { void* dummy; };
struct Base { void* vtbl; };

struct Obj {
    void* vtbl;      // +0x00
    char pad04[0x28 - 0x04];
    MemberB28 mB;    // +0x28
    char pad2C[0x48 - 0x28 - sizeof(MemberB28)];
    MemberA48 mA;    // +0x48
};

static void MemberA48_dtor(MemberA48*) { g_a48 = ++g_order; }
static void MemberB28_dtor(MemberB28*) { g_b28 = ++g_order; }
static void Base_dtor(Base*)           { g_base = ++g_order; }

static void Obj_dtor(Obj* self)
{
    MemberA48_dtor(&self->mA);
    MemberB28_dtor(&self->mB);
    self->vtbl = &VTBL_CONST;
    g_vtbl_when = g_order;
    g_vtbl_set = self->vtbl;
    Base_dtor(reinterpret_cast<Base*>(self));
}

int main()
{
    Obj o;
    o.vtbl = 0;
    Obj_dtor(&o);

    int ok = (g_a48 == 1) && (g_b28 == 2) && (g_base == 3)
          && (g_vtbl_when == 2) && (g_vtbl_set == &VTBL_CONST);
    if (ok) printf("HHQDTOR_OK\n");
    else    printf("HHQDTOR_FAIL a=%d b=%d base=%d vw=%d\n",
                   g_a48, g_b28, g_base, g_vtbl_when);
    return 0;
}