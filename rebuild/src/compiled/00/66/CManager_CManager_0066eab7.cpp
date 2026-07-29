extern "C" void __fastcall
CManager_CManager_0066eab7_ConstructScreens(
    void* screens, void*, long count, void* value, void* value_end);
extern "C" void __fastcall
CManager_CManager_0066eab7_ConstructScene(void* scene, void*);
extern "C" void __fastcall
CManager_CManager_0066eab7_InitialiseScrollIndices(
    void* manager, void*, long count);

extern "C" __declspec(naked) void* __fastcall
CManager_CManager_0066eab7(void* manager, void*, void* world)
{
    __asm
    {
        push ebp
        mov ebp, esp
        mov eax, [ebp + 8]
        push ebx
        push esi
        mov esi, ecx
        mov [esi], eax
        lea eax, [ebp + 0Bh]
        push eax
        lea eax, [ebp + 8]
        push eax
        xor ebx, ebx
        push 0Bh
        lea ecx, [esi + 4]
        mov [ebp + 8], ebx
        call CManager_CManager_0066eab7_ConstructScreens
        mov [esi + 10h], ebx
        mov [esi + 14h], ebx
        mov [esi + 18h], ebx
        mov [esi + 1Ch], ebx
        mov [esi + 20h], ebx
        mov [esi + 24h], ebx
        lea ecx, [esi + 38h]
        mov [esi + 28h], ebx
        mov [esi + 2Ch], ebx
        mov [esi + 30h], ebx
        mov [esi + 34h], bl
        call CManager_CManager_0066eab7_ConstructScene
        push 5
        mov ecx, esi
        call CManager_CManager_0066eab7_InitialiseScrollIndices
        mov eax, esi
        pop esi
        pop ebx
        pop ebp
        ret 4
    }
}
