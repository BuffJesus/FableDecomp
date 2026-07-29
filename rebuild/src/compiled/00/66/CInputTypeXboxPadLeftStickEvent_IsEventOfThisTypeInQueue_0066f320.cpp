extern "C" void __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Construct(
    void* event, void*);
extern "C" unsigned char __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Query(
    void* manager, void*, long kind, long state, void* event, long index);
extern "C" void __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320(
    void* input_type, void*, long event_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push ebx
        lea ecx, [esp + 4]
        call CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Construct
        mov eax, [esp + 40h]
        push eax
        lea ecx, [esp + 8]
        push ecx
        mov ecx, dword ptr [CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Manager]
        push 0
        push 11h
        call CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Query
        lea ecx, [esp + 4]
        mov bl, al
        call CInputTypeXboxPadLeftStickEvent_IsEventOfThisTypeInQueue_0066f320_Destroy
        mov al, bl
        pop ebx
        add esp, 34h
        ret 0ch
    }
}
