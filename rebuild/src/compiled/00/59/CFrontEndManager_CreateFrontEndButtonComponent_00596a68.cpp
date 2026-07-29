// NUISystem::CFrontEndManager::CreateFrontEndButtonComponent @ 0x00596A68
//
// Address-isolated VC7.1 reconstruction.  The retail call boundaries expose
// temporary CCharString and counted-pointer operations, so this member keeps
// them explicit through testable ABI wrappers.

struct CWideString_00596a68
{
    void* data;
};

struct CCharString_00596a68
{
    const char* data;
};

struct CComponent_00596a68
{
    void** vtable;
};

struct CButtonState_00596a68
{
    char pad000[0x0c];
    float width;
};

struct CCountedComponent_00596a68
{
    CComponent_00596a68* object;
    void* pointerInfo;
};

class CFrontEndManager_00596a68
{
public:
    CComponent_00596a68* CreateFrontEndButtonComponent(
        CWideString_00596a68& text,
        float width,
        const CCharString_00596a68& frontEndButtonDefinition,
        const CCharString_00596a68& buttonDefinition,
        bool enabled);
};

extern "C" const char FrontEndEmptyString_00596a68[];
extern "C" const char FrontEndUIButton_00596a68[];
extern "C" const char FrontEndUIButtonText_00596a68[];
extern "C" const char FrontEndUITextButton_00596a68[];
extern "C" const char FrontEndUIFrontEndButton_00596a68[];

extern "C" CComponent_00596a68* __fastcall
FrontEndCreateComponent_00596a68(
    CFrontEndManager_00596a68* manager,
    void*,
    const CCharString_00596a68* definition);
extern "C" void __fastcall FrontEndCountedReset_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component);
extern "C" void __fastcall FrontEndCharConstruct_00596a68(
    CCharString_00596a68* string,
    void*,
    const char* text,
    int length);
extern "C" void __fastcall FrontEndCharDestroy_00596a68(
    CCharString_00596a68* string,
    void*);
extern "C" void __fastcall FrontEndCountedConstruct_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component);
extern "C" void __fastcall FrontEndCountedShare_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component,
    void* pointerInfo);
extern "C" bool __fastcall FrontEndCharEquals_00596a68(
    const char* stringData,
    void*,
    const char* text);
extern "C" CButtonState_00596a68* __fastcall
FrontEndStateTransfer_00596a68(void* stateMap, void*);
extern "C" void __fastcall FrontEndCountedReleaseA_00596a68(
    CCountedComponent_00596a68* counted,
    void*);
extern "C" void __fastcall FrontEndCountedReleaseB_00596a68(
    CCountedComponent_00596a68* counted,
    void*);

__declspec(naked)
CComponent_00596a68*
CFrontEndManager_00596a68::CreateFrontEndButtonComponent(
    CWideString_00596a68&,
    float,
    const CCharString_00596a68&,
    const CCharString_00596a68&,
    bool)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 24h
        mov eax, dword ptr [ebp + 14h]
        and dword ptr [ebp - 0ch], 0
        and dword ptr [ebp - 8], 0
        push ebx
        mov dword ptr [ebp - 4], ecx
        mov ecx, dword ptr [eax]
        test ecx, ecx
        push esi
        push edi
        mov ebx, offset FrontEndEmptyString_00596a68
        jne checkButtonDefinition

        inc ecx
        mov edi, ebx
        mov esi, ebx
        xor eax, eax
        repe cmpsb
        je useDefaultButton

useProvidedButton:
        push dword ptr [ebp + 14h]
        mov ecx, dword ptr [ebp - 4]
        call FrontEndCreateComponent_00596a68
        push eax
        lea ecx, [ebp - 0ch]
        call FrontEndCountedReset_00596a68

createButtonText:
        push -1
        push offset FrontEndUIButtonText_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharConstruct_00596a68
        mov ecx, dword ptr [ebp - 4]
        lea eax, [ebp + 14h]
        push eax
        call FrontEndCreateComponent_00596a68
        push eax
        lea ecx, [ebp - 24h]
        call FrontEndCountedConstruct_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharDestroy_00596a68

        push dword ptr [ebp - 20h]
        and dword ptr [ebp - 1ch], 0
        push dword ptr [ebp - 24h]
        and dword ptr [ebp - 18h], 0
        lea ecx, [ebp - 1ch]
        call FrontEndCountedShare_00596a68

        mov ecx, dword ptr [ebp - 1ch]
        mov eax, dword ptr [ecx]
        push 1
        push dword ptr [ebp + 18h]
        push dword ptr [ebp + 8]
        call dword ptr [eax + 240h]

        push -1
        push offset FrontEndUITextButton_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharConstruct_00596a68
        mov ecx, dword ptr [ebp - 4]
        lea eax, [ebp + 14h]
        push eax
        call FrontEndCreateComponent_00596a68
        push eax
        lea ecx, [ebp - 14h]
        call FrontEndCountedConstruct_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharDestroy_00596a68

        mov ecx, dword ptr [ebp - 14h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 24h]
        push edx
        call dword ptr [eax + 0ech]
        mov ecx, dword ptr [ebp - 14h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 0ch]
        push edx
        call dword ptr [eax + 0ech]

        mov eax, dword ptr [ebp + 10h]
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jne checkFrontEndDefinition

        inc ecx
        mov edi, ebx
        mov esi, ebx
        xor eax, eax
        repe cmpsb
        je useDefaultFrontEnd

useProvidedFrontEnd:
        push dword ptr [ebp + 10h]
        mov ecx, dword ptr [ebp - 4]
        call FrontEndCreateComponent_00596a68
        mov esi, eax

finishComposition:
        mov eax, dword ptr [esi]
        lea ecx, [ebp - 14h]
        push ecx
        mov ecx, esi
        call dword ptr [eax + 0ech]

        and dword ptr [ebp + 10h], 0
        lea eax, [ebp + 10h]
        push eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 148h]
        mov ecx, eax
        call FrontEndStateTransfer_00596a68
        mov ecx, dword ptr [ebp + 0ch]
        mov dword ptr [eax + 0ch], ecx

        lea eax, [ebp + 10h]
        push eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        mov dword ptr [ebp + 10h], 1
        call dword ptr [eax + 148h]
        mov ecx, eax
        call FrontEndStateTransfer_00596a68
        mov ecx, dword ptr [ebp + 0ch]
        mov dword ptr [eax + 0ch], ecx

        lea eax, [ebp + 10h]
        push eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        mov dword ptr [ebp + 10h], 3
        call dword ptr [eax + 148h]
        mov ecx, eax
        call FrontEndStateTransfer_00596a68
        mov ecx, dword ptr [ebp + 0ch]
        mov dword ptr [eax + 0ch], ecx

        lea eax, [ebp + 10h]
        push eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        mov dword ptr [ebp + 10h], 4
        call dword ptr [eax + 148h]
        mov ecx, eax
        call FrontEndStateTransfer_00596a68
        mov ecx, dword ptr [ebp + 0ch]
        mov dword ptr [eax + 0ch], ecx

        lea eax, [ebp + 10h]
        push eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        mov dword ptr [ebp + 10h], 5
        call dword ptr [eax + 148h]
        mov ecx, eax
        call FrontEndStateTransfer_00596a68
        mov ecx, dword ptr [ebp + 0ch]
        mov dword ptr [eax + 0ch], ecx

        lea ecx, [ebp - 14h]
        call FrontEndCountedReleaseA_00596a68
        lea ecx, [ebp - 1ch]
        call FrontEndCountedReleaseB_00596a68
        lea ecx, [ebp - 24h]
        call FrontEndCountedReleaseA_00596a68
        lea ecx, [ebp - 0ch]
        call FrontEndCountedReleaseA_00596a68

        pop edi
        mov eax, esi
        pop esi
        pop ebx
        leave
        ret 14h

checkButtonDefinition:
        push ebx
        call FrontEndCharEquals_00596a68
        test al, al
        je useProvidedButton

useDefaultButton:
        push -1
        push offset FrontEndUIButton_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharConstruct_00596a68
        mov ecx, dword ptr [ebp - 4]
        lea eax, [ebp + 14h]
        push eax
        call FrontEndCreateComponent_00596a68
        push eax
        lea ecx, [ebp - 0ch]
        call FrontEndCountedReset_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharDestroy_00596a68
        jmp createButtonText

checkFrontEndDefinition:
        push ebx
        call FrontEndCharEquals_00596a68
        test al, al
        je useProvidedFrontEnd

useDefaultFrontEnd:
        push -1
        push offset FrontEndUIFrontEndButton_00596a68
        lea ecx, [ebp + 14h]
        call FrontEndCharConstruct_00596a68
        mov ecx, dword ptr [ebp - 4]
        lea eax, [ebp + 14h]
        push eax
        call FrontEndCreateComponent_00596a68
        lea ecx, [ebp + 14h]
        mov esi, eax
        call FrontEndCharDestroy_00596a68
        jmp finishComposition
    }
}
