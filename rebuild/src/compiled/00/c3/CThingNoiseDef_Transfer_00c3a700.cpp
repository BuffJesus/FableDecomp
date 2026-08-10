struct CPersistContext;

// base->TransferX()  at 0xc39d30, __fastcall(this) — no stack args
extern void __fastcall CThingNoiseDef_baseTransfer(void* self);

// sub object at this+0xc; its Transfer is a __fastcall taking (self, flag) on the stack
struct CThingNoiseSub {
    void Transfer(void* a, int flag);  // __fastcall -> ecx=this, push flag; push a
};

void* __fastcall Transfer(void* self)
{
    CThingNoiseDef_baseTransfer(self);
    ((CThingNoiseSub*)((char*)self + 0xc))->Transfer(self, 1);
    return self;
}