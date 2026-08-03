extern "C" void __fastcall CActiveFile_Cleanup(void* self);
extern "C" void __cdecl engine_free(void* p);

__declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    __asm {
        push esi
        mov esi, ecx
        call CActiveFile_Cleanup
        mov esi, dword ptr [esi]
        test esi, esi
        je short L1
        push esi
        call engine_free
        pop ecx
    L1:
        pop esi
        ret
    }
}