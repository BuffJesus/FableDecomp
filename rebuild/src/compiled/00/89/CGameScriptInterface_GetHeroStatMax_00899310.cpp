enum EHeroTrainableStatType
{
    EHeroTrainableStatType_First = 0
};

class CGSIHeroStatMax_Player;
class CGSIHeroStatMax_Node;

class CGSIHeroStatMax_Manager
{
public:
    CGSIHeroStatMax_Player* GetPlayer();
};

class CGSIHeroStatMax_Player
{
public:
    void* GetHero();
};

class CGSIHeroStatMax_Container
{
public:
    CGSIHeroStatMax_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroStatMax_Component
{
public:
    long GetStatMax(
        EHeroTrainableStatType statType);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroStatMax(
        EHeroTrainableStatType statType) const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroStatMax(
    EHeroTrainableStatType statType) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroStatMax_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroStatMax_Player::GetHero
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
        call CGSIHeroStatMax_Container::LowerBound
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
        call CGSIHeroStatMax_Component::GetStatMax
        pop ecx
        ret 4

fallback_component:
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        push ecx
        mov ecx, eax
        call CGSIHeroStatMax_Component::GetStatMax
        pop ecx
        ret 4

failed:
        or eax, -1
        pop ecx
        ret 4
    }
}
