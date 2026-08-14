// CCreatureActionBase::Update @ 0x006929a0
struct Sub;
struct CCreatureActionBase;

// vtbl: slot at +0xe0
struct VT {
    void (__fastcall *m[64])(void*);
};

struct CCreatureActionBase {
    VT* vtbl;      // +0x00
    char pad[0x5D];
    char flag;     // +0x61
};

// getter at 0xa01b50: __fastcall on (self+8) -> returns Sub*
extern "C" Sub* __fastcall getSub(void* subself);       // 0xa01b50
// target at 0x6633a0: void __fastcall on Sub*
extern "C" void __fastcall runSub(Sub* p);              // 0x6633a0

void __fastcall Update(CCreatureActionBase* self)
{
    if (self->flag) {
        char* base = (char*)self + 8;
        Sub* p = getSub((void*)base);
        runSub(p);
    } else {
        self->vtbl->m[0xe0/4](self);
    }
}