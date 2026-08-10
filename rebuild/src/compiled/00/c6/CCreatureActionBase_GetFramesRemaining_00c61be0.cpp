struct CCreatureActionBase {
    void* pad0;    // +0
    long* buf;     // +4
    long  count;   // +8
};

__declspec(naked) long __fastcall GetFramesRemaining(CCreatureActionBase* /*self ecx*/, int /*edx*/, long /*val*/)
{
    __asm {
        mov eax, dword ptr [ecx+8]
        mov edx, dword ptr [ecx+4]
        push esi
        mov esi, dword ptr [esp+8]
        mov dword ptr [edx+eax*4], esi
        inc dword ptr [ecx+8]
        pop esi
        ret 4
    }
}