extern "C" void __fastcall
CManager_Open_0066ea46_StartScene(
    void* scene, void*, unsigned long one, unsigned long zero, unsigned long colour);
extern "C" void __fastcall
CManager_Open_0066ea46_GetInventoryScreens(
    void* manager, void*, void* output);

extern "C" __declspec(naked) void __fastcall
CManager_Open_0066ea46(void* manager, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        xor cl, cl
        cmp byte ptr [esi + 34h], cl
        jne refresh
        push ecx
        fldz
        mov eax, esp
        push ecx
        push ecx
        fstp dword ptr [esp + 4]
        mov byte ptr [eax + 2], cl
        mov byte ptr [eax + 1], cl
        fld1
        mov byte ptr [eax], cl
        fstp dword ptr [esp]
        lea ecx, [esi + 38h]
        mov byte ptr [eax + 3], 0FFh
        call CManager_Open_0066ea46_StartScene

    refresh:
        lea eax, [esi + 10h]
        push eax
        mov ecx, esi
        mov byte ptr [esi + 34h], 1
        call CManager_Open_0066ea46_GetInventoryScreens
        pop esi
        ret
    }
}
