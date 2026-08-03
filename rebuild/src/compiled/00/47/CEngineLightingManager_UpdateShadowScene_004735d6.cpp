#pragma optimize("s", on)
#pragma optimize("y", off)

// CEngineLightingManager::UpdateShadowScene @ 0x004735D6  (__fastcall, ret 4)
//
// Genuine C++ decompilation.  Byte-identical (mod the two relocation-masked
// call rel32 operands) to the already-landed matching sibling at 0x00479187 --
// same 65-byte body, only the two masked call targets differ.
//
// Retail idiom:
//   push ebp / mov ebp,esp / push ecx        -> frame + one 4-byte local [ebp-4]
//   push esi / push 0x122d70e / mov esi,ecx / call <profiler>
//   and [ebp-4],0                            -> zero-init stack guard AFTER the call
//   switch (esi->mode @ +0x18)  (dec;dec;je / dec;jne):
//     case 2 -> guard.Apply(esi->p24, arg)     near local __fastcall, this=lea [ebp-4]
//     case 3 -> ((ShadowSink*)esi->p28)->Push(*(void**)arg)   __fastcall, one stack arg
//   pop esi / leave / ret 4

extern "C" void __stdcall Profile_Enter(int marker);

// Stack guard sub-object (4 bytes, zero-initialised).  Its out-of-line
// __fastcall member takes two stack args: this=lea [ebp-4], push arg, push p24.
struct ShadowGuard {
    void* slot;
    void Apply(void* a, void* b);
};

// __fastcall sink reached through esi->p28; single stack argument (*arg).
struct ShadowSink {
    void Push(void* v);
};

struct CEngineLightingManager {
    char   pad0[0x18];
    int    mode;              /* +0x18 */
    char   pad1[0x24 - 0x1c];
    void*  p24;               /* +0x24 */
    ShadowSink* p28;          /* +0x28 */

    void UpdateShadowScene(void* arg);
};

void CEngineLightingManager::UpdateShadowScene(void* arg)
{
    ShadowGuard guard;

    Profile_Enter(0x122d70e);

    guard.slot = 0;

    switch (this->mode) {
    case 2:
        guard.Apply(this->p24, arg);
        break;
    case 3:
        this->p28->Push(*(void**)arg);
        break;
    }
}