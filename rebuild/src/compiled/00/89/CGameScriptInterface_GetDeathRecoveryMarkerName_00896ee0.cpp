// CGameScriptInterface::GetDeathRecoveryMarkerName @ 0x00896EE0
// VC7.1, x86, /O2 /Oy.
//
// ABI model: ecx = interface, edx unused, [esp+4] = hidden CCharString
// return slot.  The two resolver calls locate the player creature.  A usable
// creature must have flags91 bit 0 clear and flags20 bit 4 set.  Interface key
// 4 is found with LowerBound; the selected interface's CCharString at +0x170
// is copy-constructed into the return slot.  Ineligible creatures construct
// the empty-string sentinel with length -1.
//
// Ordinary VC7.1 C++ reproduces all behavior but schedules the marker pointer
// through ecx, expanding `add ecx,170h` by one byte.  This isolated naked body
// preserves retail's eax allocation and provides exact relocation sites.

struct CCharString
{
    void* data;
};

struct CGameScriptInterfaceLayout
{
    char pad_00[0x14];
    void* playerSlot;
};

extern "C" void* __fastcall
FableGsiGetDeathRecoveryPlayerHandle(void* playerSlot, void*);
extern "C" void* __fastcall
FableGsiGetDeathRecoveryCreature(void* playerHandle, void*);
extern "C" void* __fastcall
FableGsiGetDeathRecoveryLowerBound(void* map, void*, const int* key);
extern "C" void __fastcall
FableGsiGetDeathRecoveryCopy(
    CCharString* result,
    void*,
    const CCharString* source);
extern "C" void __fastcall
FableGsiGetDeathRecoveryLiteral(
    CCharString* result,
    void*,
    const char* text,
    int length);
extern "C" const char FableGsiGetDeathRecoveryEmpty[];

extern "C" __declspec(naked) CCharString* __fastcall
CGameScriptInterface_GetDeathRecoveryMarkerName(
    const CGameScriptInterfaceLayout*,
    void*,
    CCharString*)
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        push esi
        call FableGsiGetDeathRecoveryPlayerHandle
        mov ecx, eax
        call FableGsiGetDeathRecoveryCreature
        test eax, eax
        je emptyResult
        test byte ptr [eax + 91h], 1
        jne emptyResult
        test byte ptr [eax + 20h], 10h
        je emptyResult
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call FableGsiGetDeathRecoveryLowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je useEnd
        cmp dword ptr [eax], 4
        jle haveEntry

useEnd:
        mov eax, esi

haveEntry:
        mov eax, dword ptr [eax + 4]
        mov esi, dword ptr [esp + 0Ch]
        add eax, 170h
        push eax
        mov ecx, esi
        call FableGsiGetDeathRecoveryCopy
        mov eax, esi
        pop esi
        pop ecx
        ret 4

emptyResult:
        mov esi, dword ptr [esp + 0Ch]
        push -1
        push offset FableGsiGetDeathRecoveryEmpty
        mov ecx, esi
        call FableGsiGetDeathRecoveryLiteral
        mov eax, esi
        pop esi
        pop ecx
        ret 4
    }
}
