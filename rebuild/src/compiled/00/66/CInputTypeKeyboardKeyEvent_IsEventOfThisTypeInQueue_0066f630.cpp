extern "C" void __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Construct(
    void* event, void*);
extern "C" unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Query(
    void* manager, void*, long state, long key, void* event);
extern "C" void __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630(
    void* input_type, void*, long event_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push esi
        mov esi, ecx
        lea ecx, [esp + 4]
        call CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Construct
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
        mov eax, 3
        jmp query

    state_two:
        mov eax, 2
        jmp query

    state_one:
        mov eax, 1

    query:
        mov edx, [esi + 4]
        lea ecx, [esp + 4]
        push ecx
        mov ecx, dword ptr [CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Manager]
        push edx
        push eax
        call CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Query
        test al, al
        pop esi
        lea ecx, [esp]
        je not_found
        mov eax, [esp + 40h]
        mov dword ptr [eax], 3f800000h
        call CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Destroy
        mov al, 1
        add esp, 34h
        ret 0ch

    not_found:
        call CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Destroy
        xor al, al
        add esp, 34h
        ret 0ch
    }
}
