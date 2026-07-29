// CUserProfileManager_VerifyAndLoadProfile @ 0x005957D9
// VC7.1, x86, /O2 /Oy.

extern "C" void* __fastcall FableProfileVerifyFind(void*,void*,void*);
extern "C" unsigned char __fastcall FableProfileVerifyName(void*,void*);
extern "C" void __fastcall FableProfileVerifyCopyWide(void*,void*,void*);
extern "C" void __fastcall FableProfileVerifyConstructRecord(void*,void*,void*);
extern "C" void __fastcall FableProfileVerifyInsert(void*,void*,void*,void*);
extern "C" void __fastcall FableProfileVerifyDestroyWide(void*,void*);

extern "C" __declspec(naked) unsigned char __fastcall
CUserProfileManager_VerifyAndLoadProfile_005957d9(void*,void*,void*)
{
    __asm {
        push ebp
        mov ebp,esp
        sub esp,18h
        push esi
        push dword ptr [ebp+8]
        lea esi,[ecx+0c4h]
        mov ecx,esi
        call FableProfileVerifyFind
        cmp eax,dword ptr [esi]
        je notCached
        mov al,byte ptr [eax+14h]
        jmp done
notCached:
        mov ecx,dword ptr [ebp+8]
        push ebx
        call FableProfileVerifyName
        push dword ptr [ebp+8]
        lea ecx,[ebp-8]
        mov bl,al
        call FableProfileVerifyCopyWide
        lea eax,[ebp-8]
        push eax
        lea ecx,[ebp-10h]
        mov byte ptr [ebp-4],bl
        call FableProfileVerifyConstructRecord
        lea eax,[ebp-10h]
        push eax
        lea eax,[ebp-18h]
        push eax
        mov ecx,esi
        call FableProfileVerifyInsert
        lea ecx,[ebp-10h]
        call FableProfileVerifyDestroyWide
        lea ecx,[ebp-8]
        call FableProfileVerifyDestroyWide
        mov al,bl
        pop ebx
done:
        pop esi
        leave
        ret 4
    }
}
