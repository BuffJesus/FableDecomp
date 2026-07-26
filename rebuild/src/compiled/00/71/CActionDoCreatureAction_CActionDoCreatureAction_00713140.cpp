struct CActionDoCreatureAction {
    void* vtbl;      // +0x00
    int   f4;        // +0x04
    char  f8;        // +0x08
    char  f9;        // +0x09
    int   fc;        // +0x0c
    char  f10;       // +0x10
    char  pad[3];
    int   f14;       // +0x14
    int   f18;       // +0x18
};

extern void __fastcall Base_ctor(void* self);
extern void* const g_vtbl_1265838;

struct Sub {
    int a;
    int b;
    void Init(void* arg);
};

typedef void* (__fastcall *VFn)(void* self);

void* __fastcall CActionDoCreatureAction_CActionDoCreatureAction(CActionDoCreatureAction* self, int /*edx*/, void* param1)
{
    Base_ctor(self);
    self->f4 = 0;
    self->f8 = 0;
    self->f9 = 0;
    self->vtbl = (void*)&g_vtbl_1265838;
    int* p = &self->f14;
    p[0] = 0;
    p[1] = 0;
    self->fc = 0;
    self->f10 = 0;
    void* r = (*(VFn*)((char*)*(void**)param1 + 0x2c))(param1);
    ((Sub*)&self->f14)->Init(r);
    return self;
}