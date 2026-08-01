// CEngineLightingManager::UpdateShadowScene @ 0x004735D6
// VC7.1, x86, /O2 /Oy.
//
// PDB ABI:
//   void __fastcall UpdateShadowScene(void* arg)
//
// Retail body: a leading scope/trace guard call taking a masked constant,
// a zeroed [ebp-4] local, then a dispatch on the shadow-scene mode field at
// this+0x18 (mode 2 vs mode 3).  Mode 3 forwards *arg to the +0x28 sub-object
// helper; mode 2 forwards (arg, this+0x24) to a masked near helper with the
// local as its this.  Any other mode falls through to the epilogue.
//
// Reconstructed as __declspec(naked) __fastcall to reproduce the exact
// __fastcall register usage (this in ecx) and the one-argument stack pop
// (ret 4).  All call/jmp rel32 and the pushed absolute constant are
// relocation-masked in parity.

extern "C" void __fastcall FableShadowSceneGuard(void* self, void*);
extern "C" void __fastcall FableShadowSceneMode3(void* obj, void*, void* value);
extern "C" void __fastcall FableShadowSceneMode2(void* local, void*, void* arg, void* field);
extern "C" const int g_FableShadowSceneGuardCookie;

extern "C" __declspec(naked) void __fastcall
CEngineLightingManager_UpdateShadowScene_004735d6(void* self, void*, void* arg)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        push offset g_FableShadowSceneGuardCookie
        mov esi, ecx
        call FableShadowSceneGuard
        and dword ptr [ebp - 4], 0
        mov eax, dword ptr [esi + 18h]
        dec eax
        dec eax
        je mode2
        dec eax
        jne done
        mov eax, dword ptr [ebp + 8]
        push dword ptr [eax]
        mov ecx, dword ptr [esi + 28h]
        call FableShadowSceneMode3
        jmp done

    mode2:
        push dword ptr [ebp + 8]
        lea ecx, [ebp - 4]
        push dword ptr [esi + 24h]
        call FableShadowSceneMode2

    done:
        pop esi
        leave
        ret 4
    }
}