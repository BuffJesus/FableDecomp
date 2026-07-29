// NUISystem::CFrontEndList::InitialiseOffsets @ 0x0054C480
// VC7.1, x86, /O2 /Oy.
//
// Unlike CList's count-dependent implementation, the front-end override
// ignores its child-count argument.  It sends 1.0f to virtual slot +0x258,
// constructs the exact zero C2DVector local, then passes that local to slot
// +0x25c.  This naked body retains retail's stack/local schedule and ret 4 ABI.

extern "C" __declspec(naked) void __fastcall
CFrontEndList_InitialiseOffsets_0054c480(void*, void*, unsigned long)
{
    __asm
    {
        sub esp, 8
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi]
        push 03f800000h
        mov dword ptr [esp + 8], 0
        mov dword ptr [esp + 0ch], 0
        call dword ptr [eax + 258h]
        mov edx, dword ptr [esi]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        call dword ptr [edx + 25ch]
        pop esi
        add esp, 8
        ret 4
    }
}
