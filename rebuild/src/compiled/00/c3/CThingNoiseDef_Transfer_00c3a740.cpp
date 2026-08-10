// CThingNoiseDef::Transfer  @ 00c3a740  (23 bytes)
// push esi; mov esi,ecx; call 0xc3a110(this=esi);
// push 1; push esi; lea ecx,[esi+0x14]; call 0xc39610(this=esi+0x14, esi, 1);
// mov eax,esi; pop esi; ret

struct CPersistContext;
struct CThingNoiseDef;

// base-class persist call: void __fastcall(CThingNoiseDef*) -- no stack args
void __fastcall BaseChain(CThingNoiseDef* self);

// subobject at +0x14 with a real member method (native thiscall: this->ecx, args->stack)
struct SubObj {
    char pad[4];
    void M(CThingNoiseDef* a, int b);
};

struct CThingNoiseDef {
    char pad[0x14];
    SubObj sub;
};

CThingNoiseDef* __fastcall Transfer(CThingNoiseDef* self, CPersistContext* ctx)
{
    BaseChain(self);
    self->sub.M(self, 1);
    return self;
}