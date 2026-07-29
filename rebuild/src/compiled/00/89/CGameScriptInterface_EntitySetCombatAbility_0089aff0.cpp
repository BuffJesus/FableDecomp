enum ETCInterfaceType
{
    ETCInterfaceType_CombatAbility = 0x30
};

struct CCombatAbilityKeyPair
{
    ETCInterfaceType m_Key;
    void*             m_pValue;
};

struct CCombatAbilityMap
{
    CCombatAbilityKeyPair* m_pBegin;
    CCombatAbilityKeyPair* m_pEnd;
};

struct CScriptThing
{
    void** m_pVTable;
};

struct CGameScriptInterface;

extern "C" CCombatAbilityKeyPair* __fastcall
Sub_FindCombatAbilityInterface(
    CCombatAbilityMap* pMap,
    int edxUnused,
    const ETCInterfaceType* pKey);

extern "C" void __fastcall Sub_SetCombatAbility(
    void* pInterface,
    int edxUnused,
    float ability,
    int enabled);

__declspec(naked)
void __fastcall CGameScriptInterface_EntitySetCombatAbility(
    const CGameScriptInterface* self,
    int edxUnused,
    const CScriptThing* pThing,
    float ability)
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        test dword ptr [eax + 24h], 10000h
        je finished

        push esi
        lea ecx, [esp + 8]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 30h
        call Sub_FindCombatAbilityInterface
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 30h
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov eax, dword ptr [eax + 4]
        mov edx, dword ptr [esp + 0Ch]
        push 1
        push edx
        mov ecx, eax
        call Sub_SetCombatAbility
        pop esi
finished:
        ret 8
    }
}
