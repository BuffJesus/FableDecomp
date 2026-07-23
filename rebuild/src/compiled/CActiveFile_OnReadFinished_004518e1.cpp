extern "C" void __fastcall CActiveFile_Helper(void* self);
extern "C" void __cdecl CActiveFile_Free(void* p);
extern "C" __declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    __asm {
        push esi
        mov esi, ecx
        call CActiveFile_Helper
        mov esi, dword ptr [esi]
        test esi, esi
        je done
        push esi
        call CActiveFile_Free
        pop ecx
    done:
        pop esi
        ret
        mov eax, ecx
        xor ecx, ecx
        mov dword ptr [eax], ecx
        mov dword ptr [eax+4], ecx
        mov dword ptr [eax+8], ecx
        ret 4
        mov eax, ecx
        xor ecx, ecx
        mov dword ptr [eax], ecx
        mov dword ptr [eax+4], ecx
        mov dword ptr [eax+8], ecx
        ret 4
    }
}