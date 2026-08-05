// NUISystem::CFrontEndManager::GetProfileNames @ 0x005955AB
//
// Retail profile-file enumeration body recovered from Fable.exe. Direct calls
// are named seams so this candidate can be built and behavior-tested in
// isolation; the control flow and instruction widths remain retail-shaped.

extern "C" void FableProfileNamesCall_00412450();
extern "C" void FableProfileNamesCall_0041a540();
extern "C" void FableProfileNamesCall_0099bf30();
extern "C" void FableProfileNamesCall_0099b510();
extern "C" unsigned char FableProfileNamesCall_00999760();
extern "C" void FableProfileNamesCall_0099b6b0();
extern "C" void FableProfileNamesCall_0099ebf0();
extern "C" void FableProfileNamesCall_0099b2c0();
extern "C" void FableProfileNamesCall_0099eae0();
extern "C" unsigned char FableProfileNamesCall_00999a30();
extern "C" void FableProfileNamesCall_0099b940();
extern "C" void FableProfileNamesCall_0099aed0();
extern "C" void FableProfileNamesCall_00997620();
extern "C" void FableProfileNamesCall_0099b720();
extern "C" unsigned char FableProfileNamesCall_00999850();
extern "C" void FableProfileNamesCall_004128a0();
extern "C" void FableProfileNamesCall_00bfe9f9();

extern "C" unsigned long FableProfileNamesGlobal_005955ab;
extern "C" const unsigned char FableProfileNamesDefaultPattern_005955ab[];
extern "C" const unsigned char FableProfileNamesWildcard_005955ab[];
extern "C" const unsigned char FableProfileNamesEmpty_005955ab[];

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GetProfileNames_005955ab(void*)
{
    __asm
    {
        push ebp
        lea ebp, [esp - 3d0h]
        sub esp, 450h
        mov ecx, dword ptr [ebp + 3d8h]
        push dword ptr [ecx + 4]
        mov eax, dword ptr [FableProfileNamesGlobal_005955ab]
        push dword ptr [ecx]
        mov dword ptr [ebp + 3cch], eax
        call FableProfileNamesCall_00412450

        push offset FableProfileNamesDefaultPattern_005955ab
        lea ecx, [ebp - 54h]
        call FableProfileNamesCall_0041a540
        mov edx, eax
        lea ecx, [ebp - 58h]
        call FableProfileNamesCall_0099bf30
        lea ecx, [ebp - 54h]
        call FableProfileNamesCall_0099b510
        lea eax, [ebp + 1c0h]
        push eax
        lea edx, [ebp - 78h]
        lea ecx, [ebp - 58h]
        call FableProfileNamesCall_00999760
        test al, al
        je profileNamesDone

        push ebx
        push esi
profileNamesLoop:
        lea eax, [ebp + 1c0h]
        push eax
        lea ecx, [ebp - 50h]
        call FableProfileNamesCall_0099b6b0
        push -1
        push offset FableProfileNamesWildcard_005955ab
        lea ecx, [ebp - 64h]
        call FableProfileNamesCall_0099ebf0
        lea eax, [ebp - 70h]
        push eax
        lea ecx, [ebp - 50h]
        call FableProfileNamesCall_0099b2c0
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jne havePathBegin
        xor eax, eax
        jmp havePathEnd
havePathBegin:
        mov eax, dword ptr [ecx + 4]
havePathEnd:
        mov esi, dword ptr [ebp - 64h]
        test esi, esi
        jne haveNameBegin
        xor edx, edx
        jmp haveNameEnd
haveNameBegin:
        mov edx, dword ptr [esi + 4]
haveNameEnd:
        sub eax, edx
        mov edx, eax
        js invalidName
        test esi, esi
        jne nameData
        mov esi, offset FableProfileNamesEmpty_005955ab
        jmp compareName
nameData:
        mov esi, dword ptr [esi]
compareName:
        test ecx, ecx
        mov eax, offset FableProfileNamesEmpty_005955ab
        je compareLoop
        mov eax, dword ptr [ecx]
compareLoop:
        add eax, edx
compareNameBytes:
        mov dl, byte ptr [eax]
        mov cl, dl
        cmp dl, byte ptr [esi]
        jne compareDifferent
        test cl, cl
        je compareEqual
        mov dl, byte ptr [eax + 1]
        mov cl, dl
        cmp dl, byte ptr [esi + 1]
        jne compareDifferent
        inc eax
        inc eax
        inc esi
        inc esi
        test cl, cl
        jne compareNameBytes
compareEqual:
        xor eax, eax
        jmp compareResult
compareDifferent:
        sbb eax, eax
        sbb eax, -1
compareResult:
        test eax, eax
        sete bl
        jmp compareFinished
invalidName:
        xor bl, bl
compareFinished:
        lea ecx, [ebp - 70h]
        call FableProfileNamesCall_0099eae0
        lea ecx, [ebp - 64h]
        call FableProfileNamesCall_0099eae0
        test bl, bl
        jne skipCurrent
        lea ecx, [ebp - 58h]
        call FableProfileNamesCall_00999a30
        test al, al
        jne skipCurrent
        push offset FableProfileNamesWildcard_005955ab
        lea ecx, [ebp - 50h]
        call FableProfileNamesCall_0099b940
        lea ecx, [ebp - 54h]
        call FableProfileNamesCall_0099aed0
        push 0
        push 0
        lea eax, [ebp - 54h]
        push eax
        xor edx, edx
        lea ecx, [ebp - 50h]
        call FableProfileNamesCall_00997620
        push offset FableProfileNamesWildcard_005955ab
        lea eax, [ebp - 50h]
        push eax
        lea ecx, [ebp - 6ch]
        call FableProfileNamesCall_0099b720
        mov edx, eax
        lea ecx, [ebp - 5ch]
        call FableProfileNamesCall_0099bf30
        lea ecx, [ebp - 6ch]
        call FableProfileNamesCall_0099b510
        lea eax, [ebp - 4ch]
        push eax
        lea edx, [ebp - 80h]
        lea ecx, [ebp - 5ch]
        call FableProfileNamesCall_00999760
        test al, al
        je afterNested
nestedLoop:
        lea ecx, [ebp - 60h]
        call FableProfileNamesCall_0099aed0
        lea eax, [ebp + 1c0h]
        push eax
        lea ecx, [ebp - 68h]
        call FableProfileNamesCall_0099b6b0
        push 0
        lea eax, [ebp - 60h]
        push eax
        push 0
        xor edx, edx
        lea ecx, [ebp - 68h]
        call FableProfileNamesCall_00997620
        lea ecx, [ebp - 68h]
        call FableProfileNamesCall_0099b510
        mov ecx, dword ptr [ebp + 3d8h]
        lea eax, [ebp - 60h]
        push eax
        call FableProfileNamesCall_004128a0
        lea eax, [ebp - 4ch]
        push eax
        lea edx, [ebp - 80h]
        lea ecx, [ebp - 5ch]
        call FableProfileNamesCall_00999850
        lea ecx, [ebp - 60h]
        mov bl, al
        call FableProfileNamesCall_0099b510
        test bl, bl
        jne nestedLoop
afterNested:
        lea ecx, [ebp - 5ch]
        call FableProfileNamesCall_0099b510
        lea ecx, [ebp - 54h]
        call FableProfileNamesCall_0099b510
skipCurrent:
        lea eax, [ebp + 1c0h]
        push eax
        lea edx, [ebp - 78h]
        lea ecx, [ebp - 58h]
        call FableProfileNamesCall_00999850
        lea ecx, [ebp - 50h]
        mov bl, al
        call FableProfileNamesCall_0099b510
        test bl, bl
        jne profileNamesLoop
        pop esi
        pop ebx
profileNamesDone:
        lea ecx, [ebp - 58h]
        call FableProfileNamesCall_0099b510
        mov ecx, dword ptr [ebp + 3cch]
        call FableProfileNamesCall_00bfe9f9
        add ebp, 3d0h
        leave
        ret 4
    }
}
