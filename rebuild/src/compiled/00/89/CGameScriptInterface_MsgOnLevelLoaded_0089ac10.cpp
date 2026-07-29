struct CGameScriptInterface
{
    void* m_pVTable;
    void* m_pWorld;
};

extern "C" unsigned long __fastcall
Sub_GetLevelLoadedRangeBegin(const CGameScriptInterface* self);

extern "C" unsigned long __fastcall
Sub_GetLevelLoadedRangeEnd(const CGameScriptInterface* self);

extern "C" long __fastcall Sub_ForEachLevelLoadedMessage(
    void* pManager,
    void* pCallbackRegister,
    void* pFilter,
    void* pCallback);

__declspec(naked)
bool __fastcall CGameScriptInterface_MsgOnLevelLoaded(
    const CGameScriptInterface* self,
    int edxUnused,
    void* pMessages)
{
    __asm
    {
        sub esp, 18h
        push ebx
        push esi
        push edi
        mov esi, ecx
        call Sub_GetLevelLoadedRangeBegin
        mov ecx, esi
        mov edi, eax
        call Sub_GetLevelLoadedRangeEnd
        mov esi, dword ptr [esi + 4]
        mov ebx, dword ptr [esp + 28h]
        mov dword ptr [esp + 10h], eax
        lea eax, [esp + 28h]
        lea edx, [esp + 14h]
        mov dword ptr [esp + 1Ch], eax
        lea ecx, [esp + 0Ch]
        push edx
        lea eax, [esp + 20h]
        mov dword ptr [esp + 24h], ecx
        mov ecx, dword ptr [esi + 60h]
        push eax
        mov dword ptr [esp + 1Ch], esi
        mov dword ptr [esp + 20h], ebx
        mov dword ptr [esp + 14h], edi
        mov dword ptr [esp + 30h], 24h
        call Sub_ForEachLevelLoadedMessage
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [eax]
        xor ecx, ecx
        cmp edx, eax
        pop edi
        setne cl
        pop esi
        mov al, cl
        pop ebx
        add esp, 18h
        ret 4
    }
}
