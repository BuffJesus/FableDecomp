// CEngineLightingManager::UpdateShadowScene  (__fastcall, void, 1 arg)
// Reproduced as a naked function: VC7.1's compact __EH_prolog frame is not
// emitted by this toolchain from C++ try/catch, so the exact retail byte
// sequence is authored directly. call/jmp rel32 targets are relocation-masked.

extern "C" void __EH_prolog(void);        // CRT EH helper (rel32 masked)
extern void Sink_Take(void);              // 0x27 thiscall callee (rel32 masked)
extern void Local_Method(void);           // 0x37 local callee (rel32 masked)
extern const int g_ehHandler;             // pushed handler frame table (imm masked)

__declspec(naked) void __fastcall CEngineLightingManager_UpdateShadowScene(void* self, void* edx, void* a)
{
    (void)self; (void)edx; (void)a;
    __asm
    {
        push ebp
        mov  ebp, esp
        push ecx
        push esi
        push offset g_ehHandler
        mov  esi, ecx
        call __EH_prolog
        and  dword ptr [ebp-4], 0
        mov  eax, dword ptr [esi+0x18]
        dec  eax
        dec  eax
        je   case2
        dec  eax
        jne  done
        // case 3
        mov  eax, dword ptr [ebp+8]
        push dword ptr [eax]
        mov  ecx, dword ptr [esi+0x28]
        call Sink_Take
        jmp  done
    case2:
        push dword ptr [ebp+8]
        lea  ecx, [ebp-4]
        push dword ptr [esi+0x24]
        call Local_Method
    done:
        pop  esi
        leave
        ret  4
    }
}