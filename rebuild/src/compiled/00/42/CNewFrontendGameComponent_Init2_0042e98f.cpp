// CNewFrontendGameComponent::Init2 @ 0x0042E98F
//
// Exact retail startup coordinator. The helper bodies remain named seams
// until native bank/manager ownership is linked into the executable. This
// preserves the retail stack choreography around the compressed font stream
// and the two temporary bank-pointer constructors.

extern "C" unsigned char FableInit2FrontendFlag_01375448;
extern "C" void* FableInit2DisplayManager_013b8390;

extern "C" void FableInit2GetManager_00595582();
extern "C" void FableInit2InitialiseGraphics_0042ddb3();
extern "C" void FableInit2InitFrontendSounds_0042e17c();
extern "C" void FableInit2InitialiseInput_0042dec5();
extern "C" void FableInit2SetCurrentThingManager_00aeaa80();
extern "C" void* FableInit2OperatorNew_00bfea1a();
extern "C" void FableInit2CompressedDataCtor_009c85a0();
extern "C" void FableInit2CountedPointerReset_00403e40();
extern "C" void* FableInit2GetFontBankName_00415260();
extern "C" void FableInit2StringCons_0099eae0();
extern "C" void FableInit2StringPairSwap_0042fb40();
extern "C" void* FableInit2GetPreMainMemoryUsed_0043368d();
extern "C" void FableInit2BankPointerCtor_00428905();
extern "C" void FableInit2BankPointerCtor_0041ba30();
extern "C" void FableInit2ManagerInit_005958f5();
extern "C" void FableInit2ManagerInit2_00598a1c();
extern "C" void FableInit2SetDisplayMode_009bff40();

extern "C" __declspec(naked) void __fastcall
CNewFrontendGameComponent_Init2_0042e98f(void*)
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
        mov esi, ecx

        call FableInit2GetManager_00595582
        mov dword ptr [esi + 0b4h], eax
        mov dword ptr [eax + 01ch], esi
        cmp byte ptr [FableInit2FrontendFlag_01375448], 0
        mov ecx, dword ptr [esi + 0b4h]
        sete al
        mov byte ptr [ecx + 0c0h], al

        mov ecx, esi
        call FableInit2InitialiseGraphics_0042ddb3
        mov ecx, esi
        call FableInit2InitFrontendSounds_0042e17c
        mov ecx, esi
        call FableInit2InitialiseInput_0042dec5
        mov ecx, dword ptr [esi + 040h]
        call FableInit2SetCurrentThingManager_00aeaa80

        push 0214h
        call FableInit2OperatorNew_00bfea1a
        test eax, eax
        pop ecx
        je init2_no_stream
        mov ecx, eax
        call FableInit2CompressedDataCtor_009c85a0
        jmp short init2_stream_ready
init2_no_stream:
        xor eax, eax
init2_stream_ready:
        lea edi, [esi + 060h]
        push eax
        mov ecx, edi
        call FableInit2CountedPointerReset_00403e40
        mov edi, dword ptr [edi]
        mov ebx, dword ptr [edi]
        push 0100h
        lea ecx, [ebp - 04h]
        call FableInit2GetFontBankName_00415260
        push eax
        mov ecx, edi
        call dword ptr [ebx + 04h]
        lea ecx, [ebp - 04h]
        call FableInit2StringCons_0099eae0
        call FableInit2StringPairSwap_0042fb40

        call FableInit2GetPreMainMemoryUsed_0043368d
        push eax
        push dword ptr [esi + 068h]
        lea eax, [esi + 048h]
        push ecx
        push ecx
        mov ecx, esp
        push eax
        call FableInit2BankPointerCtor_00428905
        push ecx
        push ecx
        lea eax, [esi + 040h]
        mov ecx, esp
        push eax
        call FableInit2BankPointerCtor_0041ba30

        mov ecx, dword ptr [esi + 0b4h]
        call FableInit2ManagerInit_005958f5
        cmp byte ptr [esi + 0144h], 0
        mov ecx, dword ptr [esi + 0b4h]
        sete al
        push eax
        call FableInit2ManagerInit2_00598a1c

        mov ecx, dword ptr [FableInit2DisplayManager_013b8390]
        push 03ch
        push 0
        push 010h
        lea eax, [ebp - 08h]
        push eax
        mov dword ptr [ebp - 08h], 0400h
        mov dword ptr [ebp - 04h], 0300h
        call FableInit2SetDisplayMode_009bff40

        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
