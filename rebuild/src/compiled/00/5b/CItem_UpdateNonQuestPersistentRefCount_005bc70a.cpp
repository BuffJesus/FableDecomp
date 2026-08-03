__declspec(naked) void UpdateNonQuestPersistentRefCount()
{
    __asm {
        mov eax, dword ptr [ecx+8]
        xor ecx, ecx
        cmp eax, ecx
        je   L_done
        cmp byte ptr [eax+0x18], cl
        jne  L_done
        cmp byte ptr [esp+4], cl
        je   L_reset
        inc  dword ptr [eax+0x1c]
        jmp  L_done
    L_reset:
        mov  dword ptr [eax+0x1c], ecx
    L_done:
        ret  4
    }
}