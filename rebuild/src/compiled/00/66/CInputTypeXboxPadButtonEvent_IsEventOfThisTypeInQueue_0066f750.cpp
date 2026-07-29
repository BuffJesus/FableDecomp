extern "C" void __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Construct(
    void* event, void*);
extern "C" unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Query(
    void* manager, void*, long kind, long button, void* event, long index);
extern "C" float __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_GetStrength(
    void* event, void*);
extern "C" void __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750(
    void* input_type, void*, long general_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push esi
        mov esi, ecx
        lea ecx, [esp + 4]
        call CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Construct
        mov eax, [esp + 3ch]
        dec eax
        je state_one
        dec eax
        je state_two
        dec eax
        je state_three
        xor eax, eax
        jmp query

    state_three:
        mov eax, 15h
        jmp query

    state_two:
        mov eax, 14h
        jmp query

    state_one:
        mov eax, 13h

    query:
        mov ecx, [esp + 40h]
        push ecx
        mov ecx, [esi + 4]
        lea edx, [esp + 8]
        push edx
        push ecx
        mov ecx, dword ptr [CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Manager]
        push eax
        call CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Query
        test al, al
        pop esi
        lea ecx, [esp]
        je not_found
        call CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_GetStrength
        mov edx, [esp + 40h]
        lea ecx, [esp]
        fstp dword ptr [edx]
        call CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Destroy
        mov al, 1
        add esp, 34h
        ret 0ch

    not_found:
        call CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Destroy
        xor al, al
        add esp, 34h
        ret 0ch
    }
}
