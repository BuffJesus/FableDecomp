class CScriptThing
{
};

class CGSIReadable_Node;

class CGSIReadable_Temporary
{
public:
    CGSIReadable_Temporary();
    ~CGSIReadable_Temporary();
    void Destruct();

private:
    unsigned long m_Data;
};

class CCharString
{
public:
    CGSIReadable_Temporary BuildTemporary() const;
};

class CGSIReadable_Container
{
public:
    CGSIReadable_Node* LowerBound(const long* pKey);
};

class CGSIReadable_Destination
{
public:
    void Assign(const CGSIReadable_Temporary& value);
};

class CGameScriptInterface
{
public:
    virtual void SetReadableObjectTextTag(
        const CScriptThing& entity,
        const CCharString& textTag) const;
};

__declspec(naked)
void CGameScriptInterface::SetReadableObjectTextTag(
    const CScriptThing& entity,
    const CCharString& textTag) const
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        mov ecx, dword ptr [eax + 20h]
        test ch, 20h
        je finished

        push esi
        lea ecx, [esp + 8]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 0Dh
        call CGSIReadable_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 0Dh
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [esp + 0Ch]
        mov esi, dword ptr [eax + 4]
        lea edx, [esp + 8]
        push edx
        call CCharString::BuildTemporary
        push eax
        lea ecx, [esi + 18h]
        call CGSIReadable_Destination::Assign
        lea ecx, [esp + 8]
        call CGSIReadable_Temporary::Destruct
        pop esi

finished:
        ret 8
    }
}
