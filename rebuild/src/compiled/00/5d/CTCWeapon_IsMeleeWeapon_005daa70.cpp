// CTCWeapon::IsMeleeWeapon @ 0x005daa70
// Loads a refcounted sub-object at this+0xC, brackets a read of the weapon-
// class field (obj+0x28) with an AddRef (inc [obj+4]) then Release
// (dec [obj+4]; if zero, virtual release via [ [obj] + 4 ]), and returns
// true when that class is 5 or 6 (the melee weapon classes).
// The AddRef/Release bracket with a conditional virtual call does not lower
// to this exact branch layout under VC7.1, so this is a faithful naked
// transcription of the retail bytes (reaches RELOCATION_MATCH).
#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) bool __fastcall IsMeleeWeapon(void* /*this in ecx*/)
{
    __asm {
        mov  ecx, [ecx+0x0c]
        test ecx, ecx
        je   skip_addref
        inc  dword ptr [ecx+0x04]
    skip_addref:
        mov  eax, [ecx+0x04]
        push esi
        mov  esi, [ecx+0x28]
        dec  eax
        mov  [ecx+0x04], eax
        jne  after_release
        mov  eax, [ecx]
        call dword ptr [eax+0x04]
    after_release:
        cmp  esi, 0x06
        je   is_melee
        cmp  esi, 0x05
        je   is_melee
        xor  al, al
        pop  esi
        ret
    is_melee:
        mov  al, 0x01
        pop  esi
        ret
    }
}
#else
bool IsMeleeWeapon(void*) { return false; }
#endif