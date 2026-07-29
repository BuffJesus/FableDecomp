// NUISystem::CFrontEndScreen::Initialise @ 0x0054E4B0
// VC7.1, x86, /O2 /Oy.

extern "C" void __fastcall FableFrontEndScreenInitialiseBase(void*,void*);
extern "C" void __fastcall FableFrontEndScreenConstructAcceptName(void*,void*,const char*,long);
extern "C" void __fastcall FableFrontEndScreenPublishAccept(void*,void*);
extern "C" const char FableFrontEndScreenAcceptName[];

extern "C" __declspec(naked) void __fastcall
CFrontEndScreen_Initialise_0054e4b0(void*,void*)
{
    __asm {
        push esi
        push edi
        mov esi,ecx
        call FableFrontEndScreenInitialiseBase
        mov eax,dword ptr [esi+30h]
        mov edi,dword ptr [esi]
        push ecx
        mov ecx,esp
        push -1
        push offset FableFrontEndScreenAcceptName
        mov dword ptr [esi+15ch],eax
        call FableFrontEndScreenConstructAcceptName
        mov ecx,esi
        call dword ptr [edi+0ch]
        test eax,eax
        mov dword ptr [esi+168h],eax
        je done
        mov edx,eax
        mov ecx,esi
        pop edi
        pop esi
        jmp FableFrontEndScreenPublishAccept
done:
        pop edi
        pop esi
        ret
    }
}
