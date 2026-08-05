// CNewFrontendGameComponent::InitialiseInput @ 0x0042DEC5
// VC7.1, x86, /O2 /Oy.
//
// Retail creates/attaches the input primitive through the engine primitive
// chain and publishes the returned input object at component offset 0x68.

extern "C" unsigned long __fastcall
FableNewFrontendInitialiseInputAddChildPrimitive(void*, void*);

extern "C" __declspec(naked) void __fastcall
CNewFrontendGameComponent_InitialiseInput_0042dec5(void*, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        call FableNewFrontendInitialiseInputAddChildPrimitive
        mov eax, dword ptr [eax + 58h]
        mov dword ptr [esi + 68h], eax
        pop esi
        ret
    }
}
