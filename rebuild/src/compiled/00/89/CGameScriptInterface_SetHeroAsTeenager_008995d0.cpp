class CGSIHeroTeenager_Player;
class CGSIHeroTeenager_Node;

class CGSIHeroTeenager_Manager
{
public:
    CGSIHeroTeenager_Player* GetPlayer();
};

class CGSIHeroTeenager_Player
{
public:
    void* GetHero();
};

class CGSIHeroTeenager_Container
{
public:
    CGSIHeroTeenager_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroTeenager_Morph
{
public:
    void SetAsTeenager(bool teenager);
};

class CGSIHeroTeenager_Stats
{
public:
    void ForceStaminaTo(float stamina);
    void SoberUp();
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAsTeenager(
        bool teenager) const;
};

__declspec(naked)
void CGameScriptInterface::SetHeroAsTeenager(
    bool teenager) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        push edi
        call CGSIHeroTeenager_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroTeenager_Player::GetHero
        mov edi, eax
        test edi, edi
        je finished
        test byte ptr [edi + 91h], 1
        jne finished

        test byte ptr [edi + 20h], 8
        push esi
        je fallback_morph
        lea eax, [esp + 8]
        lea esi, [edi + 44h]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 3
        call CGSIHeroTeenager_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_morph_sentinel
        cmp dword ptr [eax], 3
        jle have_morph
use_morph_sentinel:
        mov eax, esi
have_morph:
        mov eax, dword ptr [eax + 4]
        jmp dispatch_morph

fallback_morph:
        mov eax, dword ptr [esp + 10h]
dispatch_morph:
        mov ecx, dword ptr [esp + 10h]
        push ecx
        mov ecx, eax
        call CGSIHeroTeenager_Morph::SetAsTeenager

        test byte ptr [edi + 20h], 10h
        je finished_components
        lea edx, [esp + 10h]
        lea esi, [edi + 44h]
        push edx
        mov ecx, esi
        mov dword ptr [esp + 14h], 4
        call CGSIHeroTeenager_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_stats_sentinel
        cmp dword ptr [eax], 4
        jle have_stats
use_stats_sentinel:
        mov eax, esi
have_stats:
        mov esi, dword ptr [eax + 4]
        push 3F800000h
        mov ecx, esi
        call CGSIHeroTeenager_Stats::ForceStaminaTo
        mov ecx, esi
        call CGSIHeroTeenager_Stats::SoberUp
finished_components:
        pop esi
finished:
        pop edi
        pop ecx
        ret 4
    }
}
