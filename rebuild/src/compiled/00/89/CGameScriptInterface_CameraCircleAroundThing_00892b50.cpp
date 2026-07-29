class C3DVector;
class CScriptThing;

extern void* CGSICameraCircleThing_ModuleVftable[];
extern unsigned char CGSICameraCircleThing_Callback;

void* __cdecl CGSICameraCircleThing_Allocate(unsigned long size);

class CGSICameraCircleThing_ModuleBase
{
public:
    void Construct();
};

class CGSICameraCircleThing_Receiver
{
public:
    void SetScriptedCameraMode(
        void* pModule,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraCircleAroundThing(
        const CScriptThing& target,
        const C3DVector& offset,
        float speed) const;
};

__declspec(naked)
void CGameScriptInterface::CameraCircleAroundThing(
    const CScriptThing& target,
    const C3DVector& offset,
    float speed) const
{
    __asm
    {
        push ecx
        mov eax, dword ptr [ecx + 4]
        mov ecx, dword ptr [eax + 34h]
        mov edx, dword ptr [ecx]
        push esi
        push edi
        lea eax, [esp + 8]
        push eax
        call dword ptr [edx + 24h]

        push 60h
        call CGSICameraCircleThing_Allocate
        mov esi, eax
        add esp, 4
        test esi, esi
        je no_module_storage
        mov ecx, dword ptr [esp + 10h]
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 2Ch]
        mov ecx, esi
        mov edi, eax
        call CGSICameraCircleThing_ModuleBase::Construct
        mov eax, dword ptr [esp + 14h]
        mov dword ptr [esi], offset CGSICameraCircleThing_ModuleVftable
        mov dword ptr [esi + 4Ch], edi
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 50h], ecx
        mov edx, dword ptr [eax + 4]
        mov ecx, dword ptr [esp + 18h]
        mov dword ptr [esi + 54h], edx
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esi + 58h], eax
        mov dword ptr [esi + 5Ch], ecx
        jmp have_module
no_module_storage:
        xor esi, esi
have_module:
        sub esp, 8
        test esi, esi
        mov edi, esp
        mov dword ptr [edi], esi
        je no_module

        push 0Ch
        call CGSICameraCircleThing_Allocate
        add esp, 4
        test eax, eax
        je no_callback_storage
        mov ecx, dword ptr [edi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset CGSICameraCircleThing_Callback
        mov dword ptr [eax + 8], ecx
        mov dword ptr [edi + 4], eax
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCircleThing_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch

no_callback_storage:
        xor eax, eax
        mov dword ptr [edi + 4], eax
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCircleThing_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch

no_module:
        mov dword ptr [edi + 4], 0
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCircleThing_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch
    }
}
