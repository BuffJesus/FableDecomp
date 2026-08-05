// NUISystem::CFrontEndManager::RefreshAvailableProfiles @ 0x00596CCE
//
// Address-isolated VC7.1 reconstruction from the installed retail binary.
// The instruction sequence is retained; direct calls are named by their
// retail address so the candidate can be parity-checked without guessing at
// private class names. Virtual calls remain the retail vtable dispatches.

struct CWideString_00596cce
{
    void* data;
};

struct CProfileVector_00596cce
{
    CWideString_00596cce* begin;
    CWideString_00596cce* end;
    CWideString_00596cce* capacity;
};

class CFrontEndManager_00596cce
{
public:
    char pad000[0x54];
    char usedComponents[0x0c];

    long RefreshAvailableProfiles();
};

extern "C" void __fastcall FableRefreshProfilesCall_005955ab(
    void*, void*, CProfileVector_00596cce*);
extern "C" void* __cdecl FableRefreshProfilesCall_0041e5f2();
extern "C" void** __fastcall FableRefreshProfilesCall_0059b5d7(
    void*, void*, const int*);
extern "C" void __fastcall FableRefreshProfilesCall_0099ebf0(
    void*, void*, const char*, int);
extern "C" void __fastcall FableRefreshProfilesCall_0042bed4(void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0042cd84(
    void*, void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0042abca(void*, void*);
extern "C" void* __fastcall FableRefreshProfilesCall_0041db1d(
    void*, void*, void*, bool);
extern "C" void __fastcall FableRefreshProfilesCall_00429c15(
    void*, void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0099eae0(void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0059aa86(
    void*, void*, void*, void*);
extern "C" void* __fastcall FableRefreshProfilesCall_00596a68(
    void*, void*, void*, float, void*, void*, bool);
extern "C" unsigned char __fastcall FableRefreshProfilesCall_005957d9(
    void*, void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0099b6a0(void*, void*);
extern "C" void* __cdecl FableRefreshProfilesCall_00bfea1a(unsigned int);
extern "C" void __fastcall FableRefreshProfilesCall_0042be50();
extern "C" void __fastcall FableRefreshProfilesCall_0042aa29(
    void*, void*, void*);
extern "C" void* __cdecl FableRefreshProfilesCall_0040d2a0();
extern "C" void* __fastcall FableRefreshProfilesCall_004082d0(void*);
extern "C" void __fastcall FableRefreshProfilesCall_0059a994(void*, void*);
extern "C" void* __fastcall FableRefreshProfilesCall_0054dae0(
    void*, void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0042b646(
    void*, void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_00429160(void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_004291de(void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_00412130(void*, void*);
extern "C" void __fastcall FableRefreshProfilesCall_0099b510(void*, void*);

extern "C" const char FableRefreshProfilesDefinition_01252428[];
extern "C" const char FableRefreshProfilesNewButton_0125240c[];
extern "C" const char FableRefreshProfilesEmpty_0122d70e[];
extern "C" const char FableRefreshProfilesButton_012523e4[];
extern "C" const float FableRefreshProfilesSpacing_01248018;

__declspec(naked)
long CFrontEndManager_00596cce::RefreshAvailableProfiles()
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 64h
        push ebx
        xor eax, eax
        push esi
        mov dword ptr [ebp - 64h], eax
        mov dword ptr [ebp - 60h], eax
        mov dword ptr [ebp - 5ch], eax
        push edi
        lea eax, [ebp - 64h]
        mov esi, ecx
        push eax
        mov dword ptr [ebp - 8h], esi
        call FableRefreshProfilesCall_005955ab

        call FableRefreshProfilesCall_0041e5f2
        mov dword ptr [ebp - 18h], eax
        lea eax, [ebp - 1ch]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 1ch], 7
        call FableRefreshProfilesCall_0059b5d7
        mov edi, dword ptr [eax]
        mov ebx, dword ptr [edi]

        push ecx
        mov ecx, esp
        push -1
        mov esi, offset FableRefreshProfilesDefinition_01252428
        push esi
        call FableRefreshProfilesCall_0099ebf0
        mov ecx, edi
        call dword ptr [ebx + 0ch]
        mov ebx, eax
        test ebx, ebx
        mov dword ptr [ebp - 1ch], ebx
        je createReplacementList

        lea ecx, [ebp - 40h]
        call FableRefreshProfilesCall_0042bed4
        mov ebx, dword ptr [ebx]
        push ecx
        push ecx
        lea eax, [ebp - 40h]
        mov ecx, esp
        push eax
        mov dword ptr [ebp - 40h], 1
        call FableRefreshProfilesCall_0042cd84
        mov ecx, dword ptr [ebp - 1ch]
        call dword ptr [ebx + 0f8h]
        lea ecx, [ebp - 3ch]
        call FableRefreshProfilesCall_0042abca

createReplacementList:
        or ebx, -1
        push ebx
        push esi
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099ebf0
        mov ecx, dword ptr [ebp - 18h]
        xor esi, esi
        push esi
        lea eax, [ebp - 0ch]
        push eax
        call FableRefreshProfilesCall_0041db1d
        push eax
        lea ecx, [ebp - 58h]
        call FableRefreshProfilesCall_00429c15
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099eae0

        push dword ptr [ebp - 54h]
        lea ecx, [ebp - 38h]
        push dword ptr [ebp - 58h]
        mov dword ptr [ebp - 38h], esi
        mov dword ptr [ebp - 34h], esi
        call FableRefreshProfilesCall_0059aa86

        mov eax, dword ptr [edi]
        lea ecx, [ebp - 58h]
        push ecx
        mov ecx, edi
        call dword ptr [eax + 0ech]

        push ebx
        push offset FableRefreshProfilesNewButton_0125240c
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099ebf0
        lea edx, [ebp - 0ch]
        lea ecx, [ebp - 20h]
        call FableRefreshProfilesCall_0099b6a0
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099eae0

        push ebx
        push offset FableRefreshProfilesEmpty_0122d70e
        lea ecx, [ebp - 04h]
        call FableRefreshProfilesCall_0099ebf0
        push ebx
        mov esi, offset FableRefreshProfilesButton_012523e4
        push esi
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099ebf0
        fldz
        push 1
        lea eax, [ebp - 04h]
        push eax
        lea eax, [ebp - 0ch]
        push eax
        push ecx
        mov ecx, dword ptr [ebp - 8h]
        fstp dword ptr [esp]
        lea eax, [ebp - 20h]
        push eax
        call FableRefreshProfilesCall_00596a68
        push eax
        lea ecx, [ebp - 50h]
        call FableRefreshProfilesCall_00429c15
        lea ecx, [ebp - 0ch]
        call FableRefreshProfilesCall_0099eae0
        lea ecx, [ebp - 04h]
        call FableRefreshProfilesCall_0099eae0

        push 10h
        call FableRefreshProfilesCall_00bfea1a
        test eax, eax
        pop ecx
        je noNewProfileAction
        mov ecx, eax
        call FableRefreshProfilesCall_0042be50
        jmp haveNewProfileAction
noNewProfileAction:
        xor eax, eax
haveNewProfileAction:
        push eax
        lea ecx, [ebp - 48h]
        call FableRefreshProfilesCall_0042aa29
        mov eax, dword ptr [ebp - 48h]
        mov dword ptr [eax], 125h
        mov ecx, dword ptr [ebp - 50h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 48h]
        push edx
        call dword ptr [eax + 108h]
        mov ecx, dword ptr [ebp - 50h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 48h]
        push edx
        call dword ptr [eax + 11ch]
        mov ecx, dword ptr [ebp - 38h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 50h]
        push edx
        call dword ptr [eax + 0f0h]

        mov ebx, dword ptr [ebp - 60h]
        mov eax, dword ptr [ebp - 64h]
        sub ebx, eax
        sar ebx, 2
        mov dword ptr [ebp - 0ch], 42480000h
        mov dword ptr [ebp - 28h], ebx
        je finishProfilesNoLoop
        mov dword ptr [ebp - 4h], eax
        mov dword ptr [ebp - 1ch], ebx

profileLoop:
        push -1
        push offset FableRefreshProfilesEmpty_0122d70e
        lea ecx, [ebp - 14h]
        call FableRefreshProfilesCall_0099ebf0
        push -1
        push esi
        lea ecx, [ebp - 10h]
        call FableRefreshProfilesCall_0099ebf0

        fld dword ptr [ebp - 0ch]
        push 0
        lea eax, [ebp - 14h]
        push eax
        lea eax, [ebp - 10h]
        push eax
        push ecx
        mov ecx, dword ptr [ebp - 8h]
        fstp dword ptr [esp]
        push dword ptr [ebp - 4h]
        call FableRefreshProfilesCall_00596a68
        push eax
        lea ecx, [ebp - 40h]
        call FableRefreshProfilesCall_00429c15
        lea ecx, [ebp - 10h]
        call FableRefreshProfilesCall_0099eae0
        lea ecx, [ebp - 14h]
        call FableRefreshProfilesCall_0099eae0

        push 10h
        call FableRefreshProfilesCall_00bfea1a
        test eax, eax
        pop ecx
        je noProfileAction
        mov ecx, eax
        call FableRefreshProfilesCall_0042be50
        jmp haveProfileAction
noProfileAction:
        xor eax, eax
haveProfileAction:
        push eax
        lea ecx, [ebp - 30h]
        call FableRefreshProfilesCall_0042aa29
        push 0
        push dword ptr [ebp - 4h]
        lea eax, [ebp - 24h]
        push eax
        call FableRefreshProfilesCall_0040d2a0
        mov ecx, eax
        call FableRefreshProfilesCall_004082d0
        mov ecx, dword ptr [ebp - 8h]
        push eax
        call FableRefreshProfilesCall_005957d9
        lea ecx, [ebp - 24h]
        mov bl, al
        call FableRefreshProfilesCall_0099b510
        mov eax, dword ptr [ebp - 30h]
        neg bl
        push 0ch
        sbb ebx, ebx
        and ebx, 49h
        add ebx, 0dbh
        mov dword ptr [eax], ebx
        call FableRefreshProfilesCall_00bfea1a
        test eax, eax
        pop ecx
        je noProfileParameter
        push dword ptr [ebp - 4h]
        mov ecx, eax
        call FableRefreshProfilesCall_0054dae0
        mov dword ptr [ebp - 18h], eax
        jmp haveProfileParameter
noProfileParameter:
        and dword ptr [ebp - 18h], 0
haveProfileParameter:
        mov ecx, dword ptr [ebp - 30h]
        lea eax, [ebp - 18h]
        push eax
        add ecx, 4
        call FableRefreshProfilesCall_0042b646

        mov ecx, dword ptr [ebp - 40h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 30h]
        push edx
        call dword ptr [eax + 108h]
        mov ecx, dword ptr [ebp - 40h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 30h]
        push edx
        call dword ptr [eax + 11ch]
        mov ecx, dword ptr [ebp - 38h]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 40h]
        push edx
        call dword ptr [eax + 0f0h]

        fld dword ptr [ebp - 0ch]
        fadd dword ptr [FableRefreshProfilesSpacing_01248018]
        lea ecx, [ebp - 30h]
        fstp dword ptr [ebp - 0ch]
        call FableRefreshProfilesCall_00429160
        lea ecx, [ebp - 40h]
        call FableRefreshProfilesCall_004291de
        add dword ptr [ebp - 4h], 4
        dec dword ptr [ebp - 1ch]
        jne profileLoop

        mov ebx, dword ptr [ebp - 28h]
finishProfilesNoLoop:
        mov eax, dword ptr [edi]
        mov ecx, edi
        call dword ptr [eax + 0ach]
        lea ecx, [ebp - 48h]
        call FableRefreshProfilesCall_00429160
        lea ecx, [ebp - 50h]
        call FableRefreshProfilesCall_004291de
        lea ecx, [ebp - 20h]
        call FableRefreshProfilesCall_0099b510
        lea ecx, [ebp - 38h]
        call FableRefreshProfilesCall_0059a994
        lea ecx, [ebp - 58h]
        call FableRefreshProfilesCall_004291de
        lea ecx, [ebp - 64h]
        call FableRefreshProfilesCall_00412130
        pop edi
        pop esi
        mov eax, ebx
        pop ebx
        leave
        ret
    }
}
