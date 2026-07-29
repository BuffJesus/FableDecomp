// CUserProfileManager::ValidateSaveFile @ 0x0047ED0A
// VC7.1, x86, /O2 /Oy.

extern "C" void* __fastcall FableValidateSaveFind(void*,void*,void*);
extern "C" unsigned char __fastcall FableValidateSaveSignature(void*,void*);
extern "C" void __fastcall FableValidateSaveCopyWide(void*,void*,void*);
extern "C" void __fastcall FableValidateSaveConstructKey(void*,void*,void*);
extern "C" void __fastcall FableValidateSaveInsert(void*,void*,void*,void*);
extern "C" void __fastcall FableValidateSaveDestroyWide(void*,void*);

extern "C" __declspec(naked) unsigned char __fastcall
CUserProfileManager_ValidateSaveFile_0047ed0a(void*,void*,void*)
{
    __asm {
        push ebp
        mov ebp,esp
        sub esp,18h
        push esi
        push dword ptr [ebp+8]
        lea esi,[ecx+0e0h]
        mov ecx,esi
        call FableValidateSaveFind
        cmp eax,dword ptr [esi]
        je uncached
        mov al,byte ptr [eax+14h]
        jmp done
uncached:
        mov ecx,dword ptr [ebp+8]
        push ebx
        call FableValidateSaveSignature
        push dword ptr [ebp+8]
        lea ecx,[ebp-8]
        mov bl,al
        call FableValidateSaveCopyWide
        lea eax,[ebp-8]
        push eax
        lea ecx,[ebp-10h]
        mov byte ptr [ebp-4],bl
        call FableValidateSaveConstructKey
        lea eax,[ebp-10h]
        push eax
        lea eax,[ebp-18h]
        push eax
        mov ecx,esi
        call FableValidateSaveInsert
        lea ecx,[ebp-10h]
        call FableValidateSaveDestroyWide
        lea ecx,[ebp-8]
        call FableValidateSaveDestroyWide
        mov al,bl
        pop ebx
done:
        pop esi
        leave
        ret 4
    }
}
