// CGameScriptInterface::SetEnvironmentThemeWeightAllInternals @ 0x00890040
// __fastcall: this in ECX, two stack args (theme, arg2), ret 8.
// arg1 (theme) selects a theme-list; if its internal count is > 0, loops
// i=1..4 calling an interface method for each internal, threading (i, 1,
// count, arg2). arg2 is reloaded from its stack home at [esp+0x14] inside
// the loop. Emitted as naked asm to reproduce the retail register allocation
// (this->EBP, count->EDI, arg2->EBX, i->ESI) and the split live range exactly.

extern "C" int   GetThemeListForTheme(int theme);     // stdcall-style: arg pushed
extern "C" int   ThemeList_GetInternalCount(int list); // thiscall (ecx = list)
extern "C" void  ThemeInternal_SetWeight();            // thiscall (ecx = internal)

__declspec(naked) void CGameScriptInterface_SetEnvironmentThemeWeightAllInternals()
{
    __asm {
        mov  eax, [esp+4]           // theme (arg1)
        push ebp
        push edi
        push eax                    // pass theme
        mov  ebp, ecx               // this
        call GetThemeListForTheme
        mov  ecx, eax
        call ThemeList_GetInternalCount
        mov  edi, eax               // count
        test edi, edi
        jle  done
        push ebx
        mov  ebx, [esp+0x14]        // reload theme
        push esi
        mov  esi, 1
    lp:
        mov  ecx, [ebp+4]           // this->mid
        mov  ecx, [ecx+0x18]        // mid->internal
        push ebx                    // theme
        push edi                    // count
        push 1
        push esi                    // i
        call ThemeInternal_SetWeight
        inc  esi
        cmp  esi, 4
        jle  lp
        pop  esi
        pop  ebx
    done:
        pop  edi
        pop  ebp
        ret  8
    }
}