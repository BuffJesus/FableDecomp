// CDataInputStream::CDataInputStream @ 0x00993B20.
//
// LoadGameStateInternal constructs this stream base before opening and
// decoding the save chunks.  Retail constructs CBase first, clears the five
// stream cursor/source fields at +4..+0x14, installs the CDataInputStream
// vtable, and returns the original object.

extern "C" void* __fastcall
CDataInputStream_ConstructBase_00993b20(void* stream, void*);

extern "C" void* CDataInputStream_VTable_00993b20[];

extern "C" __declspec(naked) void* __fastcall
CDataInputStream_CDataInputStream_00993b20(void*, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        call CDataInputStream_ConstructBase_00993b20
        xor eax, eax
        mov dword ptr [esi + 4], eax
        mov dword ptr [esi + 8], eax
        mov dword ptr [esi + 0Ch], eax
        mov dword ptr [esi + 10h], eax
        mov dword ptr [esi + 14h], eax
        mov dword ptr [esi], offset CDataInputStream_VTable_00993b20
        mov eax, esi
        pop esi
        ret
    }
}
