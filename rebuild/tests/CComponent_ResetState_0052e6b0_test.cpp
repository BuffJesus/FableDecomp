#include <cstdio>

static char* g_block;
static int g_calls[3];

struct CComponent {
    virtual void f00(){}  virtual void f01(){}  virtual void f02(){}  virtual void f03(){}
    virtual void f04(){}  virtual void f05(){}  virtual void f06(){}  virtual void f07(){}
    virtual void f08(){}  virtual void f09(){}  virtual void f10(){}
    virtual void f11(void* p){ g_calls[1] = (int)((char*)p - g_block); }
    virtual void f12(){}
    virtual void f13(void* p){ g_calls[2] = (int)((char*)p - g_block); }
    virtual void f14(){}
    virtual void f15(void* p){ g_calls[0] = (int)((char*)p - g_block); }
    virtual void f16(){} virtual void f17(){} virtual void f18(){} virtual void f19(){}
    virtual void f20(){} virtual void f21(){} virtual void f22(){} virtual void f23(){}
    virtual void f24(){} virtual void f25(){} virtual void f26(){} virtual void f27(){}
    virtual void f28(){} virtual void f29(){} virtual void f30v(){} virtual void f31(){}
    virtual void f32(){} virtual void f33(){} virtual void f34v(){} virtual void f35(){}
    virtual void f36(){} virtual void f37(){} virtual void f38(){} virtual void f39(){}
    virtual void f40(){} virtual void f41(){} virtual void f42(){} virtual void f43(){}
    virtual void f44(){} virtual void f45(){} virtual void f46(){} virtual void f47(){}
    virtual void f48(){} virtual void f49(){}
    virtual char* f50(){ return g_block; }

    char pad[0x30 - 4];
    int f30;
    char pad2[0x98 - 0x34];
    int f98;
    int f9c;
    int fa0;
    int fa4;
    int fa8;
};

void __fastcall CComponent_ResetState(CComponent* self)
{
    char* p = self->f50();
    self->f15(p + 0x8);
    self->f11(p + 0x10);
    self->f13(p + 0x18);
    self->fa0 = 0;
    self->f98 = 0;
    self->fa8 = 0;
    self->f30 = 0;
}

int main()
{
    static char blk[0x40];
    g_block = blk;
    CComponent c;
    c.f30 = 0x1111; c.f98 = 0x2222; c.fa0 = 0x3333; c.fa8 = 0x4444;
    CComponent_ResetState(&c);
    if (c.f30==0 && c.f98==0 && c.fa0==0 && c.fa8==0 &&
        g_calls[0]==0x8 && g_calls[1]==0x10 && g_calls[2]==0x18) {
        std::printf("CComponent_0052e6b0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL f30=%d f98=%d fa0=%d fa8=%d c0=%d c1=%d c2=%d\n",
        c.f30, c.f98, c.fa0, c.fa8, g_calls[0], g_calls[1], g_calls[2]);
    return 1;
}