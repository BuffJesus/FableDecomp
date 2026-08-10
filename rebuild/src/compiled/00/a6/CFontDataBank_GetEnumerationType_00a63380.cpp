
extern "C" void __stdcall Helper_9cbd10(void* a, void* b);
__declspec(naked) void* GetEnumerationType_Font(void* /*a*/, void* /*b*/)
{
    __asm {
        mov eax, dword ptr [esp+8]
        push esi
        mov esi, dword ptr [esp+8]
        push eax
        push esi
        call Helper_9cbd10
        mov eax, esi
        pop esi
        ret 8
    }
}