// NUISystem::CFrontEndManager::GetScoreboardOptionsToProfile @ 0x00596550
// VC7.1, x86, /O2 /Oy.

extern "C" void** __fastcall FableScoreboardCommitResolve(void*,void*,const unsigned long*);
extern "C" void** __fastcall FableScoreboardCommitStackTop(void*,void*);
extern "C" void __fastcall FableScoreboardCommitConstructName(void*,void*,const char*,long);
extern "C" void* __fastcall FableScoreboardCommitGetEdit(void*,void*,void*,void*);
extern "C" void* __cdecl FableScoreboardCommitGetService();
extern "C" void __fastcall FableScoreboardCommitAssignWide(void*,void*,void*);
extern "C" void __fastcall FableScoreboardCommitDestroyWide(void*,void*);
extern "C" void __fastcall FableScoreboardCommitDestroyName(void*,void*);
extern "C" void* __fastcall FableScoreboardCommitConvertWide(void*,void*,void*);
extern "C" long __fastcall FableScoreboardCommitParseInteger(void*,void*);

extern "C" const char FableScoreboardCommitLogin[];
extern "C" const char FableScoreboardCommitPassword[];
extern "C" const char FableScoreboardCommitProxyIp[];
extern "C" const char FableScoreboardCommitProxyPort[];
extern "C" const char FableScoreboardCommitServerPort[];

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GetScoreboardOptionsToProfile_00596550(void*,void*)
{
    __asm {
        push ebp
        mov ebp,esp
        sub esp,0ch
        push esi
        push edi
        mov esi,ecx
        lea eax,[ebp-4]
        push eax
        lea ecx,[esi+54h]
        mov dword ptr [ebp-4],19h
        call FableScoreboardCommitResolve
        lea ecx,[esi+20h]
        mov edi,eax
        call FableScoreboardCommitStackTop
        mov eax,dword ptr [eax]
        cmp eax,dword ptr [edi]
        jne finished
        push ebx
        or edi,-1

        push edi
        push offset FableScoreboardCommitLogin
        lea ecx,[ebp-4]
        call FableScoreboardCommitConstructName
        lea eax,[ebp-4]
        push eax
        lea eax,[ebp-8]
        push eax
        mov ecx,esi
        call FableScoreboardCommitGetEdit
        mov ebx,eax
        call FableScoreboardCommitGetService
        push ebx
        lea ecx,[eax+110h]
        call FableScoreboardCommitAssignWide
        lea ecx,[ebp-8]
        call FableScoreboardCommitDestroyWide
        lea ecx,[ebp-4]
        call FableScoreboardCommitDestroyName

        push edi
        push offset FableScoreboardCommitPassword
        lea ecx,[ebp-4]
        call FableScoreboardCommitConstructName
        lea eax,[ebp-4]
        push eax
        lea eax,[ebp-8]
        push eax
        mov ecx,esi
        call FableScoreboardCommitGetEdit
        mov ebx,eax
        call FableScoreboardCommitGetService
        push ebx
        lea ecx,[eax+114h]
        call FableScoreboardCommitAssignWide
        lea ecx,[ebp-8]
        call FableScoreboardCommitDestroyWide
        lea ecx,[ebp-4]
        call FableScoreboardCommitDestroyName

        push edi
        push offset FableScoreboardCommitProxyIp
        lea ecx,[ebp-4]
        call FableScoreboardCommitConstructName
        lea eax,[ebp-4]
        push eax
        lea eax,[ebp-8]
        push eax
        mov ecx,esi
        call FableScoreboardCommitGetEdit
        mov ebx,eax
        call FableScoreboardCommitGetService
        push ebx
        lea ecx,[eax+118h]
        call FableScoreboardCommitAssignWide
        lea ecx,[ebp-8]
        call FableScoreboardCommitDestroyWide
        lea ecx,[ebp-4]
        call FableScoreboardCommitDestroyName

        push edi
        push offset FableScoreboardCommitProxyPort
        lea ecx,[ebp-4]
        call FableScoreboardCommitConstructName
        lea eax,[ebp-8]
        push eax
        lea eax,[ebp-4]
        push eax
        lea eax,[ebp-0ch]
        push eax
        mov ecx,esi
        call FableScoreboardCommitGetEdit
        mov ecx,eax
        call FableScoreboardCommitConvertWide
        mov ecx,eax
        call FableScoreboardCommitParseInteger
        mov ebx,eax
        call FableScoreboardCommitGetService
        lea ecx,[ebp-8]
        mov dword ptr [eax+11ch],ebx
        call FableScoreboardCommitDestroyName
        lea ecx,[ebp-0ch]
        call FableScoreboardCommitDestroyWide
        lea ecx,[ebp-4]
        call FableScoreboardCommitDestroyName

        push edi
        push offset FableScoreboardCommitServerPort
        lea ecx,[ebp-4]
        call FableScoreboardCommitConstructName
        lea eax,[ebp-0ch]
        push eax
        lea eax,[ebp-4]
        push eax
        lea eax,[ebp-8]
        push eax
        mov ecx,esi
        call FableScoreboardCommitGetEdit
        mov ecx,eax
        call FableScoreboardCommitConvertWide
        mov ecx,eax
        call FableScoreboardCommitParseInteger
        mov esi,eax
        call FableScoreboardCommitGetService
        lea ecx,[ebp-0ch]
        mov dword ptr [eax+120h],esi
        call FableScoreboardCommitDestroyName
        lea ecx,[ebp-8]
        call FableScoreboardCommitDestroyWide
        lea ecx,[ebp-4]
        call FableScoreboardCommitDestroyName
        pop ebx
finished:
        pop edi
        pop esi
        leave
        ret
    }
}
