#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dc600

struct RefObj;

struct RefVtbl {
    void (__fastcall *dtor0)(RefObj*);        // slot 0 [vtbl+0]
    void (__fastcall *release)(RefObj*);      // slot 1 [vtbl+4]
};

struct RefObj {
    RefVtbl* vtbl;   // +0
    int      refcnt; // +4
};

struct C3DAnimationBlendState {
    char    pad[0xc];  // +0
    RefObj* ptr;       // +0xc
};

// base/member dtor chained via tail jmp @ 0x00686830
extern void __fastcall Base_dtor(C3DAnimationBlendState* self);

void __fastcall dtor_C3DAnimationBlendState(C3DAnimationBlendState* self)
{
    RefObj* p = self->ptr;            // mov ecx,[esi+0xc]
    if (p) {                          // test ecx,ecx / je
        if (--p->refcnt == 0) {       // dec [ecx+4] / jne
            p->vtbl->release(p);      // mov eax,[ecx]; call [eax+4]
        }
        self->ptr = 0;                // and [esi+0xc],0
    }
    Base_dtor(self);                  // mov ecx,esi; jmp 0x686830
}