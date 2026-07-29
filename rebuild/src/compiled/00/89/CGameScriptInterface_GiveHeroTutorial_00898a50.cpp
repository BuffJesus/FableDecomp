enum ETutorialCategory
{
    ETutorialCategory_First = 0
};

class CGSITutorial_Player;
class CGSITutorial_Node;

class CGSITutorial_Manager
{
public:
    CGSITutorial_Player* GetPlayer();
};

class CGSITutorial_Player
{
public:
    void* GetHero();
};

class CGSITutorial_Container
{
public:
    CGSITutorial_Node* LowerBound(const long* pKey);
};

class CGSITutorial_Component
{
public:
    bool GiveTutorial(
        ETutorialCategory category,
        bool force);
};

class CGameScriptInterface
{
public:
    virtual bool GiveHeroTutorial(
        ETutorialCategory category) const;
};

__declspec(naked)
bool CGameScriptInterface::GiveHeroTutorial(
    ETutorialCategory category) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSITutorial_Manager::GetPlayer
        mov ecx, eax
        call CGSITutorial_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        mov ecx, dword ptr [eax + 24h]
        test ch, 2
        je failed

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 29h
        call CGSITutorial_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 29h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [eax + 4]
        test ecx, ecx
        pop esi
        je failed
        mov edx, dword ptr [esp + 8]
        push 0
        push edx
        call CGSITutorial_Component::GiveTutorial
        pop ecx
        ret 4

failed:
        xor al, al
        pop ecx
        ret 4
    }
}
