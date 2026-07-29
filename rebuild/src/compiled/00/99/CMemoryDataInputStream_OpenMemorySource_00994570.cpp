// CMemoryDataInputStream::OpenMemorySource @ 0x00994570.
//
// After both save chunks are decompressed, LoadGameStateInternal binds their
// combined byte vector to its already-constructed memory stream.  Retail
// resets both positions, exposes the full source as the current chunk, and
// retains the original source pointer.

extern "C" __declspec(naked) void __fastcall
CMemoryDataInputStream_OpenMemorySource_00994570(
    void*,
    void*,
    void*,
    unsigned long)
{
    __asm
    {
        mov edx, dword ptr [esp + 8]
        xor eax, eax
        mov dword ptr [ecx + 4], eax
        mov dword ptr [ecx + 10h], eax
        mov eax, dword ptr [esp + 4]
        mov dword ptr [ecx + 0Ch], eax
        mov dword ptr [ecx + 8], edx
        mov dword ptr [ecx + 14h], edx
        mov dword ptr [ecx + 18h], eax
        ret 8
    }
}
