class CGSIHeroRenown_Player;
class CGSIHeroRenown_Node;

class CGSIHeroRenown_Manager
{
public:
    CGSIHeroRenown_Player* GetPlayer();
};

class CGSIHeroRenown_Player
{
public:
    void* GetHero();
};

class CGSIHeroRenown_Container
{
public:
    CGSIHeroRenown_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroRenown_Component
{
public:
    void AddRenown(long renownPoints);
};

class CGameScriptInterface
{
public:
    virtual void GiveHeroRenownPoints(
        long renownPoints) const;
};

__declspec(naked)
void CGameScriptInterface::GiveHeroRenownPoints(
    long renownPoints) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroRenown_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroRenown_Player::GetHero
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
        call CGSIHeroRenown_Container::LowerBound
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
        call CGSIHeroRenown_Component::AddRenown
        pop ecx
        ret 4

fallback_component:
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        push ecx
        mov ecx, eax
        call CGSIHeroRenown_Component::AddRenown
finished:
        pop ecx
        ret 4
    }
}
