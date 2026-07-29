class CScriptThing;

class CGSIForceLookCamera_Node;

class CGSIForceLookCamera_Container
{
public:
    CGSIForceLookCamera_Node* LowerBound(const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual void EntityForceToLookAtCamera(
        const CScriptThing& entity,
        bool enabled) const;
};

__declspec(naked)
void CGameScriptInterface::EntityForceToLookAtCamera(
    const CScriptThing& entity,
    bool enabled) const
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 8]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je finished

        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        mov ecx, dword ptr [eax + 20h]
        test ch, 1
        je finished

        lea esi, [eax + 44h]
        lea eax, [esp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 8
        call CGSIForceLookCamera_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 8
        jle selected_node
use_sentinel:
        mov eax, esi
selected_node:
        mov eax, dword ptr [eax + 4]
        mov cl, byte ptr [esp + 0Ch]
        mov byte ptr [eax + 34h], cl
finished:
        pop esi
        ret 8
    }
}
