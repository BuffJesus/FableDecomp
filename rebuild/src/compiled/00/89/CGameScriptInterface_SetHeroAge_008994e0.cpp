class CGSIHeroAge_Player;
class CGSIHeroAge_Node;

class CGSIHeroAge_Manager
{
public:
    CGSIHeroAge_Player* GetPlayer();
};

class CGSIHeroAge_Player
{
public:
    void* GetHero();
};

class CGSIHeroAge_Container
{
public:
    CGSIHeroAge_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroAge_Component
{
public:
    void SetAge(float age);
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAge(float age) const;
};

__declspec(naked)
void CGameScriptInterface::SetHeroAge(float age) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroAge_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroAge_Player::GetHero
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        test byte ptr [eax + 20h], 10h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CGSIHeroAge_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [esp + 0Ch]
        mov eax, dword ptr [eax + 4]
        pop esi
        push ecx
        mov ecx, eax
        call CGSIHeroAge_Component::SetAge
        pop ecx
        ret 4

fallback_component:
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        push ecx
        mov ecx, eax
        call CGSIHeroAge_Component::SetAge
finished:
        pop ecx
        ret 4
    }
}
