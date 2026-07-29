enum ETCInterfaceType
{
    ETCInterfaceType_Narrator = 0x0F
};

struct CNarratorKeyPair
{
    ETCInterfaceType m_Key;
    void*             m_pValue;
};

struct CNarratorMap
{
    CNarratorKeyPair* m_pBegin;
    CNarratorKeyPair* m_pEnd;
};

struct CScriptThing
{
    void** m_pVTable;
};

struct CGameScriptInterface;

extern "C" CNarratorKeyPair* __fastcall Sub_FindNarratorInterface(
    CNarratorMap* pMap,
    int edxUnused,
    const ETCInterfaceType* pKey);

__declspec(naked)
void __fastcall CGameScriptInterface_EntityResetAsPureAINarrator(
    const CGameScriptInterface* self,
    int edxUnused,
    const CScriptThing* pThing)
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 8]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je finished
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        cmp byte ptr [eax + 10h], 1
        jne finished
        mov ecx, dword ptr [eax + 20h]
        test ch, ch
        jns finished

        lea esi, [eax + 44h]
        lea eax, [esp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 0Fh
        call Sub_FindNarratorInterface
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 0Fh
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov eax, dword ptr [eax + 4]
        mov byte ptr [eax + 34h], 1
finished:
        pop esi
        ret 4
    }
}
