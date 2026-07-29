extern "C" void __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Construct(
    void* event, void*);
extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Translate(
    void* input_type, void*, long general_type);
extern "C" unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Query(
    void* manager, void*, long event_type, void* event);
extern "C" void __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Destroy(
    void* event, void*);
extern "C" void*
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Manager;

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0(
    void* input_type, void*, long general_type, long index, float* strength)
{
    __asm
    {
        sub esp, 34h
        push esi
        mov esi, ecx
        lea ecx, [esp + 4]
        call CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Construct
        mov ecx, [esp + 3ch]
        lea eax, [esp + 4]
        push eax
        push ecx
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Translate
        mov ecx, dword ptr [CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Manager]
        push eax
        call CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Query
        test al, al
        pop esi
        lea ecx, [esp]
        je not_found
        mov edx, [esp + 40h]
        mov dword ptr [edx], 3f800000h
        call CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Destroy
        mov al, 1
        add esp, 34h
        ret 0ch

    not_found:
        call CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Destroy
        xor al, al
        add esp, 34h
        ret 0ch
    }
}
