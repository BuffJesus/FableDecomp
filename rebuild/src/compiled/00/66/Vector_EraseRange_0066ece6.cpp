extern "C" void* __fastcall
Vector_EraseRange_0066ece6_Move(
    void* source_begin, void* source_end, void* destination);

extern "C" __declspec(naked) void* __fastcall
Vector_EraseRange_0066ece6(
    void* vector, void*, void* erase_begin, void* erase_end)
{
    __asm
    {
        push esi
        push dword ptr [esp + 8]
        mov esi, ecx
        mov edx, [esi + 4]
        mov ecx, [esp + 10h]
        call Vector_EraseRange_0066ece6_Move
        mov [esi + 4], eax
        mov eax, [esp + 8]
        pop esi
        ret 8
    }
}
