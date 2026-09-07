// Vector_ConstructInitialized_C2DLineF @ 0x00a80360
// Allocates count * 16 bytes and copy-initializes every slot from the compiler's
// 16-byte local seed (POD default-initialization leaves that seed unspecified).

extern "C" void VectorC2DLineF_Allocate_00bfea0e();

extern "C" __declspec(naked) void* __fastcall
Vector_ConstructInitialized_C2DLineF_00a80360(void* vector, void*, unsigned long count)
{
    __asm {
        sub esp, 10h
        push esi
        mov esi, ecx
        push edi
        mov edi, [esp+1ch]
        test edi, edi
        mov dword ptr [esi], 0
        mov dword ptr [esi+4], 0
        mov dword ptr [esi+8], 0
        je no_allocation
        mov eax, edi
        shl eax, 4
        push eax
        call VectorC2DLineF_Allocate_00bfea0e
        add esp, 4
        jmp allocation_ready
no_allocation:
        xor eax, eax
allocation_ready:
        mov ecx, edi
        shl ecx, 4
        add ecx, eax
        test edi, edi
        mov [esi], eax
        mov [esi+4], eax
        mov [esi+8], ecx
        jbe initialized
initialize_loop:
        test eax, eax
        je advance
        mov edx, [esp+8]
        mov ecx, [esp+0ch]
        mov [eax], edx
        mov edx, [esp+10h]
        mov [eax+4], ecx
        mov ecx, [esp+14h]
        mov [eax+8], edx
        mov [eax+0ch], ecx
advance:
        add eax, 10h
        dec edi
        jne initialize_loop
initialized:
        mov [esi+4], eax
        pop edi
        mov eax, esi
        pop esi
        add esp, 10h
        ret 4
    }
}