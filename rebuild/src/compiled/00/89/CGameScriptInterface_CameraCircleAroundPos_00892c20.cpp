class C3DVector;

extern void* CGSICameraCirclePos_ModuleVftable[];
extern unsigned char CGSICameraCirclePos_Callback;

void* __cdecl CGSICameraCirclePos_Allocate(unsigned long size);

class CGSICameraCirclePos_ModuleBase
{
public:
    void Construct();
};

class CGSICameraCirclePos_Receiver
{
public:
    void SetScriptedCameraMode(
        void* pModule,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraCircleAroundPos(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        float speed) const;
};

__declspec(naked)
void CGameScriptInterface::CameraCircleAroundPos(
    const C3DVector& firstPosition,
    const C3DVector& secondPosition,
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

        push 68h
        call CGSICameraCirclePos_Allocate
        mov esi, eax
        add esp, 4
        test esi, esi
        je no_module_storage
        mov ecx, esi
        call CGSICameraCirclePos_ModuleBase::Construct
        mov eax, dword ptr [esp + 10h]
        mov dword ptr [esi], offset CGSICameraCirclePos_ModuleVftable
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 4Ch], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [esi + 50h], edx
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esi + 54h], eax
        mov eax, dword ptr [esp + 14h]
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 58h], ecx
        mov edx, dword ptr [eax + 4]
        mov ecx, dword ptr [esp + 18h]
        mov dword ptr [esi + 5Ch], edx
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esi + 60h], eax
        mov dword ptr [esi + 64h], ecx
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
        call CGSICameraCirclePos_Allocate
        add esp, 4
        test eax, eax
        je no_callback_storage
        mov ecx, dword ptr [edi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset CGSICameraCirclePos_Callback
        mov dword ptr [eax + 8], ecx
        mov dword ptr [edi + 4], eax
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCirclePos_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch

no_callback_storage:
        xor eax, eax
        mov dword ptr [edi + 4], eax
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCirclePos_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch

no_module:
        mov dword ptr [edi + 4], 0
        mov ecx, dword ptr [esp + 10h]
        call CGSICameraCirclePos_Receiver::SetScriptedCameraMode
        pop edi
        pop esi
        pop ecx
        ret 0Ch
    }
}
