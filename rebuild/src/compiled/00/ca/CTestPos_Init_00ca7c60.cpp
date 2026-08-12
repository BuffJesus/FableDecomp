// CTestPos::Init  0x00ca7c60
// Retail loads 'this' from the stack ([esp+8] after push esi) and does ret 4,
// so it is a callee-cleanup (stdcall) free function taking the object pointer
// on the stack, not an ecx-thiscall. Model it that way to reproduce the bytes.

struct IProvider
{
    virtual int v0()=0;
    virtual int v1()=0;
    virtual int v2()=0;
    virtual int v3()=0;
    virtual int v4()=0;   // slot 4  -> [vtbl+0x10]
};

struct CTestPos
{
    int         field_0;   // +0
    int         field_4;   // +4
    IProvider*  field_8;   // +8
    int         field_c;   // +0xc
};

int __stdcall CTestPos_Init(CTestPos* self)
{
    IProvider* p = self->field_8;
    self->field_4 = 0;
    self->field_c = p->v4();
    return 0;
}