// CVirtualKeyboard::GetProfileName @ 0x00851890
// VC7.1, x86, /O2 /Oy.

extern "C" long __fastcall FableProfileNameLength(void*,void*);
extern "C" const unsigned short* __fastcall FableProfileNameData(void*,void*);
extern "C" void __fastcall FableProfileNameSubstring(void*,void*,void*,long,long);
extern "C" void __fastcall FableProfileNameDestroy(void*,void*);

extern "C" __declspec(naked) void* __fastcall
CVirtualKeyboard_GetProfileName_00851890(void*,void*,void*)
{
    __asm {
        push ecx
        mov ecx,dword ptr [ecx+0ch]
        mov eax,dword ptr [ecx]
        push esi
        push edi
        lea edx,[esp+8]
        push edx
        call dword ptr [eax+238h]
        lea ecx,[esp+8]
        xor edi,edi
        call FableProfileNameLength
        mov esi,eax
        lea ecx,[esp+8]
        dec esi
        call FableProfileNameLength
        test eax,eax
        jle trailing
        mov edi,edi
leading:
        lea ecx,[esp+8]
        call FableProfileNameData
        cmp word ptr [eax+edi*2],20h
        jne trailing
        lea ecx,[esp+8]
        inc edi
        call FableProfileNameLength
        cmp edi,eax
        jl leading
trailing:
        cmp esi,edi
        jle makeSubstring
trimTail:
        lea ecx,[esp+8]
        call FableProfileNameData
        cmp word ptr [eax+esi*2],20h
        jne makeSubstring
        dec esi
        cmp esi,edi
        jg trimTail
makeSubstring:
        inc esi
        push esi
        mov esi,dword ptr [esp+14h]
        push edi
        push esi
        lea ecx,[esp+14h]
        call FableProfileNameSubstring
        lea ecx,[esp+8]
        call FableProfileNameDestroy
        pop edi
        mov eax,esi
        pop esi
        pop ecx
        ret 4
    }
}
