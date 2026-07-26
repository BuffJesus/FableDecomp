// CTCCreatureOpinionOfHero::IsWillingToStartFollowingHero @ 0x006818a0
// __fastcall, returns bool. Reconstructed to VC7.1 byte parity.
//
// Behaviour: loads member ptr [this+4] as the argument, fetches a manager
// object via a global accessor, calls a state getter on that manager, then
// dispatches a switch on the returned state code (a jump table over cases
// 1..15) to decide whether the creature will start following the hero.

extern "C" void* CTCCreatureOpinionOfHero_GetManagerOwner(void);   // call at +0x04
// __fastcall state getter (this in ecx, one stack arg, callee-cleans). Modelled
// as __stdcall so the pushed argument is cleaned by the callee.
int __stdcall CTCCreatureOpinionOfHero_GetCreatureState(void* arg);  // call at +0x0f

extern unsigned char kIsWillingCaseIndex[15];  // byte index table @ 0x6818d8
extern void*         kIsWillingJumpTable[];     // dword jump table  @ 0x6818d0

__declspec(naked) bool __fastcall
CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero(void* self)
{
    __asm {
        push esi
        mov  esi, dword ptr [ecx+4]
        call CTCCreatureOpinionOfHero_GetManagerOwner
        mov  eax, dword ptr [eax+0x74]
        push esi
        mov  ecx, eax
        call CTCCreatureOpinionOfHero_GetCreatureState
        dec  eax
        cmp  eax, 0x0e
        pop  esi
        ja   short lfalse
        movzx eax, byte ptr [eax + kIsWillingCaseIndex]
        jmp  dword ptr [eax*4 + kIsWillingJumpTable]
    ltrue:
        mov  al, 1
        ret
    lfalse:
        xor  al, al
        ret
    }
}