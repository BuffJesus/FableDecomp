extern "C" void* __stdcall sub_9cbd10(void* a, void* b);

__declspec(naked) void* __stdcall GetEnumerationType_Streaming(void* a, void* b)
{
    (void)a; (void)b;
    __asm {
        mov eax, dword ptr [esp+8]
        push esi
        mov esi, dword ptr [esp+8]
        push eax
        push esi
        call sub_9cbd10
        mov eax, esi
        pop esi
        ret 8
    }
}