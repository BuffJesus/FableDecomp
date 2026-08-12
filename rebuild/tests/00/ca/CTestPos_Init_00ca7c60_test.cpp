#include <stdio.h>

struct IProvider
{
    virtual int v0()=0;
    virtual int v1()=0;
    virtual int v2()=0;
    virtual int v3()=0;
    virtual int v4()=0;
};

struct CTestPos
{
    int         field_0;
    int         field_4;
    IProvider*  field_8;
    int         field_c;
};

int __stdcall CTestPos_Init(CTestPos* self)
{
    IProvider* p = self->field_8;
    self->field_4 = 0;
    self->field_c = p->v4();
    return 0;
}

struct Prov : IProvider
{
    int v0() { return 0; }
    int v1() { return 0; }
    int v2() { return 0; }
    int v3() { return 0; }
    int v4() { return 0x1234; }
};

int main()
{
    Prov provider;
    IProvider* pv = &provider;
    CTestPos t;
    t.field_0 = 111;
    t.field_4 = 999;
    t.field_8 = pv;
    t.field_c = 555;

    CTestPos* tp = &t;
    int r = CTestPos_Init(tp);
    (void)r;

    if (t.field_4 == 0 && t.field_c == 0x1234 && t.field_0 == 111)
        printf("INIT_OK\n");
    else
        printf("INIT_FAIL %d %d\n", t.field_4, t.field_c);
    return 0;
}