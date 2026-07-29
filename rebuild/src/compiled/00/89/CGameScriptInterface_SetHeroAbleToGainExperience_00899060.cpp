class CGSIGainExperience_Player;
class CGSIGainExperience_Node;

class CGSIGainExperience_Manager
{
public:
    CGSIGainExperience_Player* GetPlayer();
};

class CGSIGainExperience_Player
{
public:
    void* GetHero();
};

class CGSIGainExperience_Container
{
public:
    CGSIGainExperience_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAbleToGainExperience(
        bool ableToGainExperience) const;
};

__declspec(naked)
void CGameScriptInterface::SetHeroAbleToGainExperience(
    bool ableToGainExperience) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIGainExperience_Manager::GetPlayer
        mov ecx, eax
        call CGSIGainExperience_Player::GetHero
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        mov ecx, dword ptr [eax + 2Ch]
        test ch, 1
        je finished

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 68h
        call CGSIGainExperience_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 68h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        mov cl, byte ptr [esp + 0Ch]
        mov byte ptr [eax + 3Ch], cl
        pop esi
finished:
        pop ecx
        ret 4
    }
}
