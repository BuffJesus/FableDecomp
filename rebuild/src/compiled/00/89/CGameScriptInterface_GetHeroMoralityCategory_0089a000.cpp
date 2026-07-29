enum EMorality
{
    EMorality_Default = 3
};

class CGSIMorality_Player;
class CGSIMorality_Node;

class CGSIMorality_Manager
{
public:
    CGSIMorality_Player* GetPlayer();
};

class CGSIMorality_Player
{
public:
    void* GetHero();
};

class CGSIMorality_Container
{
public:
    CGSIMorality_Node* LowerBound(const long* pKey);
};

class CGSIMorality_Component
{
public:
    EMorality GetMoralityCategory();
};

class CGameScriptInterface
{
public:
    virtual EMorality GetHeroMoralityCategory() const;
};

__declspec(naked)
EMorality CGameScriptInterface::GetHeroMoralityCategory() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIMorality_Manager::GetPlayer
        mov ecx, eax
        call CGSIMorality_Player::GetHero
        test eax, eax
        je default_result
        test byte ptr [eax + 91h], 1
        jne default_result
        test byte ptr [eax + 20h], 10h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CGSIMorality_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        pop esi
        mov ecx, eax
        call CGSIMorality_Component::GetMoralityCategory
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        mov ecx, eax
        call CGSIMorality_Component::GetMoralityCategory
        pop ecx
        ret

default_result:
        mov eax, 3
        pop ecx
        ret
    }
}
