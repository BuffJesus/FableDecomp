#pragma optimize("g",on)
struct CPersistContext;

// base->Transfer() at 0xc39f10, __fastcall(this) - no stack args
extern void __fastcall CThingNoiseDef_baseTransfer(void* self);

// sub->Transfer(self, 1) at 0xc39450: this-in-ecx (sub), two STACK args (self, 1),
// edx untouched. Model as a real __fastcall member so VC sets ecx and pushes the
// two stack args without materializing edx.
struct CThingNoiseSub {
    void Transfer(void* a, int flag);
};

void* __fastcall Transfer(void* self)
{
    CThingNoiseDef_baseTransfer(self);
    ((CThingNoiseSub*)((char*)self + 0x14))->Transfer(self, 1);
    return self;
}