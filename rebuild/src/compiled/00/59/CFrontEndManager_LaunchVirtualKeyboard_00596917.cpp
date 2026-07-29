// NUISystem::CFrontEndManager::LaunchVirtualKeyboard @ 0x00596917
//
// The retail body routes through used-key 0x17, allocates a 16-byte keyboard,
// constructs it with a second lookup of that route component, stores it at
// manager +0x60, and calls Initialize.  The address-isolated wrappers retain
// the original VC7.1 call conventions while keeping the fixture executable.

class CComponent_00596917
{
};

struct FrontEndUsedComponentMap_00596917
{
    char storage[0x0c];
};

class CVirtualKeyboard_00596917
{
public:
    void* fields[4];
};

class CFrontEndManager_00596917
{
public:
    char pad000[0x54];
    FrontEndUsedComponentMap_00596917 usedComponents; // +0x54
    CVirtualKeyboard_00596917* virtualKeyboard;       // +0x60

    void LaunchVirtualKeyboard();
};

extern "C" CComponent_00596917** __fastcall
FrontEndUsedComponentLookup_00596917(
    FrontEndUsedComponentMap_00596917* usedComponents,
    void*,
    const int* key);
extern "C" void __fastcall FrontEndGotoNextScreen_00596917(
    CFrontEndManager_00596917* manager,
    void*,
    CComponent_00596917* component,
    bool immediate);
extern "C" void* __cdecl FrontEndKeyboardAllocate_00596917(
    unsigned int bytes);
extern "C" CVirtualKeyboard_00596917* __fastcall
FrontEndKeyboardConstruct_00596917(
    CVirtualKeyboard_00596917* keyboard,
    void*,
    CComponent_00596917* component);
extern "C" long __fastcall FrontEndKeyboardInitialize_00596917(
    CVirtualKeyboard_00596917* keyboard,
    void*);

__declspec(naked)
void CFrontEndManager_00596917::LaunchVirtualKeyboard()
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push ecx
        push ebx
        push esi
        push edi
        mov edi, ecx

        push 17h
        pop esi
        lea eax, [ebp - 4]
        lea ebx, [edi + 54h]
        push eax
        mov ecx, ebx
        mov dword ptr [ebp - 4], esi
        call FrontEndUsedComponentLookup_00596917

        push 0
        push dword ptr [eax]
        mov ecx, edi
        call FrontEndGotoNextScreen_00596917

        push 10h
        call FrontEndKeyboardAllocate_00596917
        test eax, eax
        pop ecx
        mov dword ptr [ebp - 8], eax
        je allocationFailed

        lea eax, [ebp - 4]
        push eax
        mov ecx, ebx
        mov dword ptr [ebp - 4], esi
        call FrontEndUsedComponentLookup_00596917

        push dword ptr [eax]
        mov ecx, dword ptr [ebp - 8]
        call FrontEndKeyboardConstruct_00596917
        jmp storeKeyboard

allocationFailed:
        xor eax, eax

storeKeyboard:
        mov ecx, eax
        mov dword ptr [edi + 60h], eax
        call FrontEndKeyboardInitialize_00596917

        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
