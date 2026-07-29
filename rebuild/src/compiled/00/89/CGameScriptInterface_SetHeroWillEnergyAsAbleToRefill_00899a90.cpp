class CGSIWillRefill_Player;
class CGSIWillRefill_Node;

class CGSIWillRefill_Manager
{
public:
    CGSIWillRefill_Player* GetPlayer();
};

class CGSIWillRefill_Player
{
public:
    void* GetHero();
};

class CGSIWillRefill_Container
{
public:
    CGSIWillRefill_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual void SetHeroWillEnergyAsAbleToRefill(
        bool ableToRefill) const;
};

__declspec(naked)
void CGameScriptInterface::
SetHeroWillEnergyAsAbleToRefill(
    bool ableToRefill) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIWillRefill_Manager::GetPlayer
        mov ecx, eax
        call CGSIWillRefill_Player::GetHero
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
        call CGSIWillRefill_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov cl, byte ptr [esp + 0Ch]
        mov eax, dword ptr [eax + 4]
        test cl, cl
        sete cl
        pop esi
        mov byte ptr [eax + 68h], cl
        pop ecx
        ret 4

fallback_component:
        mov cl, byte ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        test cl, cl
        sete cl
        mov byte ptr [eax + 68h], cl
finished:
        pop ecx
        ret 4
    }
}
