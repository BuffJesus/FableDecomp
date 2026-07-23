#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

struct CPersistContext {
    void PersistA(A* p);
    void PersistB(B* p);
    void PersistC(C* p);
};

struct CCarrySlotDef {
    unsigned char _pad[0x28];
    A a;   // +0x28
    B b;   // +0x2c
    C c;   // +0x30
    A a2;  // +0x34
};

void __fastcall CCarrySlotDef_Transfer(CCarrySlotDef* self, void* edx, CPersistContext* ctx)
{
    ctx->PersistA(&self->a);
    ctx->PersistB(&self->b);
    ctx->PersistC(&self->c);
    ctx->PersistA(&self->a2);
}