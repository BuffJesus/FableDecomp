extern "C" void __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Construct(
    void* event, void*);
extern "C" unsigned char __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Query(
    void* manager, void*, long kind, long state, void* event, long index);
extern "C" void __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440(
    void* input_type, void*, long event_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push ebx
        lea ecx, [esp + 4]
        call CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Construct
        mov eax, [esp + 40h]
        push eax
        lea ecx, [esp + 8]
        push ecx
        mov ecx, dword ptr [CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Manager]
        push 0
        push 0dh
        call CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Query
        lea ecx, [esp + 4]
        mov bl, al
        call CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Destroy
        mov al, bl
        pop ebx
        add esp, 34h
        ret 0ch
    }
}
