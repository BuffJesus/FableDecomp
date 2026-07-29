// CFileDataInputStream::~CFileDataInputStream @ 0x00994780.
//
// LoadGameStateInternal destroys the disk-backed stream as soon as the two
// compressed save blocks have been copied into memory.  Retail restores the
// derived vtable for Close, then the CDataInputStream vtable, and tail-calls
// the CBase destructor.

extern "C" void* CFileDataInputStream_DerivedVTable_00994780[];
extern "C" void* CFileDataInputStream_BaseVTable_00994780[];
extern "C" void __fastcall
CFileDataInputStream_Close_00994780(void*, void*);
extern "C" void __fastcall
CFileDataInputStream_DestroyBase_00994780(void*, void*);

extern "C" __declspec(naked) void __fastcall
CFileDataInputStream_dtor_00994780(void*, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov dword ptr [esi], offset CFileDataInputStream_DerivedVTable_00994780
        call CFileDataInputStream_Close_00994780
        mov dword ptr [esi], offset CFileDataInputStream_BaseVTable_00994780
        mov ecx, esi
        pop esi
        jmp CFileDataInputStream_DestroyBase_00994780
    }
}
