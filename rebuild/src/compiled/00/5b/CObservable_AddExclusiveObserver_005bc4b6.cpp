// NUISystem::CObservable::AddExclusiveObserver @ 0x005bc4b6
// __fastcall (this=ecx), 1 stack arg. Stores arg into this->m_pExclusiveObserver
// (offset +0x70) and returns the stored value in eax. ret 4.
// Retail bytes: 8b442404 894170 c20400
__declspec(naked) void AddExclusiveObserver()
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov dword ptr [ecx+0x70], eax
        ret 4
    }
}