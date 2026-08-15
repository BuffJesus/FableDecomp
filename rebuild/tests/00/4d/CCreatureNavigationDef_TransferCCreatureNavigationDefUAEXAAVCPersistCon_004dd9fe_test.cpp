#include <cstdio>

struct A { int x; };
struct B { int x; };
struct C { int x; };

int g_seq[8];
int g_n = 0;

struct CPersistContext {
    void PersistA(A* p) { g_seq[g_n++] = 1; p->x += 10; }
    void PersistB(B* p) { g_seq[g_n++] = 2; p->x += 20; }
    void PersistC(C* p) { g_seq[g_n++] = 3; p->x += 30; }
};

struct CCarrySlotDef {
    unsigned char _pad[0x28];
    A a;
    B b;
    C c;
    A a2;
};

void __fastcall CCarrySlotDef_Transfer(CCarrySlotDef* self, void* edx, CPersistContext* ctx)
{
    ctx->PersistA(&self->a);
    ctx->PersistB(&self->b);
    ctx->PersistC(&self->c);
    ctx->PersistA(&self->a2);
}

int main()
{
    CCarrySlotDef d;
    d.a.x = 0; d.b.x = 0; d.c.x = 0; d.a2.x = 0;
    CPersistContext ctx;
    CCarrySlotDef_Transfer(&d, 0, &ctx);
    if (g_n != 4) { std::printf("FAIL count %d\n", g_n); return 1; }
    if (g_seq[0]!=1 || g_seq[1]!=2 || g_seq[2]!=3 || g_seq[3]!=1) { std::printf("FAIL seq\n"); return 1; }
    if (d.a.x!=10 || d.b.x!=20 || d.c.x!=30 || d.a2.x!=10) { std::printf("FAIL vals\n"); return 1; }
    std::printf("CCarrySlotDef_00452256_TEST PASS\n");
    return 0;
}