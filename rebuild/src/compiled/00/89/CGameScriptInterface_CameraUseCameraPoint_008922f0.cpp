class CCharString;
class CScriptThing;

extern void* CGSICameraUseCameraPoint_ScriptThingVftable[];
void __cdecl CGSICameraUseCameraPoint_Free(void* pMemory);

class CGSICameraUseCameraPoint_Base
{
public:
    void Destroy();
};

class CGameScriptInterface
{
public:
    virtual void CameraUseCameraPoint(
        const CCharString& cameraPointName,
        const CScriptThing& target,
        float transitionTime,
        long firstMode,
        long secondMode) const;
};

__declspec(naked)
void CGameScriptInterface::CameraUseCameraPoint(
    const CCharString& cameraPointName,
    const CScriptThing& target,
    float transitionTime,
    long firstMode,
    long secondMode) const
{
    __asm
    {
        sub esp, 0Ch
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esp + 14h]
        mov eax, dword ptr [esi]
        push ecx
        lea edx, [esp + 8]
        push edx
        mov ecx, esi
        call dword ptr [eax + 120h]

        mov ecx, dword ptr [esp + 24h]
        mov edx, dword ptr [esp + 20h]
        mov eax, dword ptr [esi]
        push ecx
        mov ecx, dword ptr [esp + 20h]
        push edx
        mov edx, dword ptr [esp + 20h]
        push ecx
        push edx
        lea ecx, [esp + 14h]
        push ecx
        mov ecx, esi
        call dword ptr [eax + 674h]

        mov eax, dword ptr [esp + 0Ch]
        xor esi, esi
        cmp eax, esi
        mov dword ptr [esp + 4], offset CGSICameraUseCameraPoint_ScriptThingVftable
        je no_reference
        dec dword ptr [eax]
        mov eax, dword ptr [esp + 0Ch]
        cmp dword ptr [eax], esi
        jne no_reference
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov edx, dword ptr [esp + 0Ch]
        push edx
        call CGSICameraUseCameraPoint_Free
        add esp, 4
no_reference:
        lea ecx, [esp + 4]
        mov dword ptr [esp + 8], esi
        mov dword ptr [esp + 0Ch], esi
        call CGSICameraUseCameraPoint_Base::Destroy
        pop esi
        add esp, 0Ch
        ret 14h
    }
}
