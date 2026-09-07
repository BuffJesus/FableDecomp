class CGameScriptInterface {
public:
    bool GetHeroHasMurderedWife() const;
};
extern "C" void HeroResolveStage1_00449970();
extern "C" void HeroResolveStage2_00487dc0();
extern "C" void InterfaceMapLowerBound_004365b0();
extern "C" void HeroStatsHasMarriageInState_0057ac29();
bool __declspec(naked) CGameScriptInterface::GetHeroHasMurderedWife() const {
    __asm {
        push ecx
        mov ecx, dword ptr [ecx+14h]
        call HeroResolveStage1_00449970
        mov ecx, eax
        call HeroResolveStage2_00487dc0
        test eax, eax
        je no_children
        test byte ptr [eax+91h], 1
        jne no_children
        test byte ptr [eax+20h], 10h
        je no_children
        push esi
        lea esi, [eax+44h]
        lea eax, [esp+4]
        push eax
        mov ecx, esi
        mov dword ptr [esp+8], 4
        call InterfaceMapLowerBound_004365b0
        mov esi, dword ptr [esi+4]
        cmp eax, esi
        je use_end
        cmp dword ptr [eax], 4
        jle have_entry
    use_end:
        mov eax, esi
    have_entry:
        mov eax, dword ptr [eax+4]
        push 7
        mov ecx, eax
        call HeroStatsHasMarriageInState_0057ac29
        pop esi
        pop ecx
        ret
    no_children:
        xor al, al
        pop ecx
        ret
    }
}