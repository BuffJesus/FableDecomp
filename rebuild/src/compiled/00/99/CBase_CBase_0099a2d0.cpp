// CBase::CBase @ 0x0099A2D0.
//
// In LoadGameStateInternal this initializes the polymorphic file object used
// by the disk/chunk reader.  The base constructor installs only its vtable and
// returns the original object; the caller immediately layers derived fields
// and the derived vtable over it.

extern "C" void* CBase_VTable_0099a2d0[];

extern "C" __declspec(naked) void* __fastcall
CBase_CBase_0099a2d0(void*, void*)
{
    __asm
    {
        mov eax, ecx
        mov dword ptr [eax], offset CBase_VTable_0099a2d0
        ret
    }
}
