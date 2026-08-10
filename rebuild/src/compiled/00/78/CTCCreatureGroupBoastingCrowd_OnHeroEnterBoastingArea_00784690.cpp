extern "C" unsigned long g_boast_ptr;
extern void __stdcall Callee_784260(void* arg);
__declspec(naked) void __fastcall OnHeroEnterBoastingArea(void* self)
{
    __asm {
        mov eax, dword ptr [g_boast_ptr]
        mov eax, dword ptr [eax+0x68]
        cmp dword ptr [eax], eax
        je done
        mov edx, dword ptr [ecx+0x30]
        push edx
        call Callee_784260
    done:
        ret
    }
}