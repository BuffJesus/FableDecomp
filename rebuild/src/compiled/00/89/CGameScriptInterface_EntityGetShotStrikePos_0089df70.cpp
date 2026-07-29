class CScriptThing;
class C3DVector;
class CGSIShotStrike_Node;

class CGSIShotStrike_Container
{
public:
    CGSIShotStrike_Node* LowerBound(const long* pKey);
};

class CGSIShotStrike_Events
{
public:
    bool GetHitEvent(void* pEvent, void* pFilter);
};

class CGameScriptInterface
{
public:
    virtual bool EntityGetShotStrikePos(
        const CScriptThing& entity,
        C3DVector& outStrikePosition) const;
};

__declspec(naked)
bool CGameScriptInterface::EntityGetShotStrikePos(
    const CScriptThing& entity,
    C3DVector& outStrikePosition) const
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        test byte ptr [eax + 28h], 4
        je failed

        push esi
        lea ecx, [esp + 8]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 42h
        call CGSIShotStrike_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 42h
        jle selected_node
use_sentinel:
        mov eax, esi
selected_node:
        mov eax, dword ptr [eax + 4]
        lea edx, [esp + 8]
        push edx
        lea ecx, [esp + 0Ch]
        push ecx
        mov ecx, eax
        call CGSIShotStrike_Events::GetHitEvent
        test al, al
        pop esi
        je failed

        mov edx, dword ptr [esp + 4]
        mov eax, dword ptr [esp + 8]
        add edx, 8
        mov ecx, dword ptr [edx]
        mov dword ptr [eax], ecx
        mov ecx, dword ptr [edx + 4]
        mov dword ptr [eax + 4], ecx
        mov edx, dword ptr [edx + 8]
        mov dword ptr [eax + 8], edx
        mov al, 1
        ret 8
failed:
        xor al, al
        ret 8
    }
}
