enum EHeroTrainableStatType
{
    EHeroTrainableStatType_First = 0
};

class CGSIHeroStatLevel_Player;
class CGSIHeroStatLevel_Node;

class CGSIHeroStatLevel_Manager
{
public:
    CGSIHeroStatLevel_Player* GetPlayer();
};

class CGSIHeroStatLevel_Player
{
public:
    void* GetHero();
};

class CGSIHeroStatLevel_Container
{
public:
    CGSIHeroStatLevel_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroStatLevel_Component
{
public:
    long GetStatLevel(
        EHeroTrainableStatType statType);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroStatLevel(
        EHeroTrainableStatType statType) const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroStatLevel(
    EHeroTrainableStatType statType) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroStatLevel_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroStatLevel_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        mov ecx, dword ptr [eax + 2Ch]
        test ch, 1
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 68h
        call CGSIHeroStatLevel_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 68h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [esp + 0Ch]
        mov eax, dword ptr [eax + 4]
        pop esi
        push ecx
        mov ecx, eax
        call CGSIHeroStatLevel_Component::GetStatLevel
        pop ecx
        ret 4

fallback_component:
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        push ecx
        mov ecx, eax
        call CGSIHeroStatLevel_Component::GetStatLevel
        pop ecx
        ret 4

failed:
        or eax, -1
        pop ecx
        ret 4
    }
}
