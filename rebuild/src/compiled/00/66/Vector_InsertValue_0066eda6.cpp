extern "C" void __fastcall
Vector_InsertValue_0066eda6_Reallocate(
    void* vector, void*, void* position, void* value, void* value_end,
    long count, long grow);

extern "C" __declspec(naked) void __fastcall
Vector_InsertValue_0066eda6(void* vector, void*, const void* value)
{
    __asm
    {
        push ebp
        mov ebp, esp
        mov eax, [ecx + 4]
        cmp eax, [ecx + 8]
        je reallocate
        test eax, eax
        je advance
        mov edx, [ebp + 8]
        mov edx, [edx]
        mov [eax], edx

    advance:
        add dword ptr [ecx + 4], 4
        jmp done

    reallocate:
        push 1
        push 1
        lea edx, [ebp + 0Bh]
        push edx
        push dword ptr [ebp + 8]
        push eax
        call Vector_InsertValue_0066eda6_Reallocate

    done:
        pop ebp
        ret 4
    }
}
