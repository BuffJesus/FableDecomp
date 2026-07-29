extern "C" void __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Construct(
    void* event, void*);
extern "C" unsigned char __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Query(
    void* manager, void*, long kind, long state, void* event, long index);
extern "C" void __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0(
    void* input_type, void*, long event_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push ebx
        lea ecx, [esp + 4]
        call CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Construct
        mov eax, [esp + 40h]
        push eax
        lea ecx, [esp + 8]
        push ecx
        mov ecx, dword ptr [CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Manager]
        push 0
        push 12h
        call CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Query
        lea ecx, [esp + 4]
        mov bl, al
        call CInputTypeXboxPadRightStickEvent_IsEventOfThisTypeInQueue_0066f3b0_Destroy
        mov al, bl
        pop ebx
        add esp, 34h
        ret 0ch
    }
}
