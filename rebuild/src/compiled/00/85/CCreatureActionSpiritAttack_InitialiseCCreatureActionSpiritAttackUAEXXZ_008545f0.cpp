// Byte-exact reconstruction of CCreatureAction_SpiritAttack::Initialise @ 0x008545f0
// mangled: ?Initialise@CCreatureAction_SpiritAttack@@UAEXXZ  (public virtual thiscall, void(void))

extern int   g_spiritInt;    // fild dword ptr [0x1375550]
extern float g_spiritMul;    // fmul dword ptr [0x1255318]

extern "C" int __cdecl SpiritHelper(void);  // call 0x694d10 (return discarded)

struct CCreatureAction_SpiritAttack {
    // vptr occupies +0x00
    char pad[0x0C];  // +0x04 .. +0x0F
    int  field10;    // +0x10
    virtual void Initialise();
};

void CCreatureAction_SpiritAttack::Initialise()
{
    SpiritHelper();
    // call 0xbfea70 = __ftol: consumes st(0), returns eax, no stack cleanup
    this->field10 = (int)((float)g_spiritInt * g_spiritMul);
}