struct Sub { virtual void d0(); virtual void d1(); virtual void d2(); virtual void V3(int); virtual void V4(int); };
struct Result { virtual void r0(); virtual void r1(); virtual void r2(); virtual void r3(); virtual void RV4(void*); };
struct S { char pad0[4]; Sub sub; };
extern void* g_activeRedefiner;
extern void __fastcall UpdateKeyText(S*);
extern Result* __fastcall getResult();
void __fastcall CancelSelection(S* self){
    if (g_activeRedefiner == self) {
        g_activeRedefiner = 0;
        UpdateKeyText(self);
        self->sub.V4(0x21);
        self->sub.V3(0x19);
        getResult()->RV4(&self->sub);
    }
}