// CPersistContext::CPersistContext(CDataInputStream*, bool) @ 0x009BAA30.
//
// LoadGameStateInternal constructs this binary-read context immediately after
// binding the decompressed save buffer.  Retail selects read mode 2, records
// the input stream, clears the output stream/current section, and resets the
// context flag at +0x15.  The boolean argument does not alter this constructor.

extern "C" __declspec(naked) void* __fastcall
CPersistContext_CPersistContext_009baa30(
    void*,
    void*,
    void*,
    bool)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [ecx + 24h], eax
        xor eax, eax
        mov dword ptr [ecx + 18h], 2
        mov dword ptr [ecx + 28h], eax
        mov dword ptr [ecx], eax
        mov byte ptr [ecx + 15h], al
        ret 8
    }
}
