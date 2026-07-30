// CCreatureAction_FireMissileWeapon::FrameUpdate @ 0x0059b61c
// __fastcall push_back fast-path: if room, store *arg into slot and advance; else tail-call grow.
extern "C" void slowpath_grow_0059b576(void);

__declspec(naked) void FrameUpdate(void* /*arg*/)
{
    __asm {
        mov edx, dword ptr [ecx+0x18]
        mov eax, dword ptr [ecx+0x10]
        sub edx, 4
        cmp eax, edx
        je  do_grow
        test eax, eax
        je  advance
        mov edx, dword ptr [esp+4]
        mov edx, dword ptr [edx]
        mov dword ptr [eax], edx
    advance:
        add dword ptr [ecx+0x10], 4
        ret 4
    do_grow:
        jmp slowpath_grow_0059b576
    }
}