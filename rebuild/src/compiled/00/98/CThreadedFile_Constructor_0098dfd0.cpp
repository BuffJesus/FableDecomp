#include "fable_threaded_file.h"

extern "C" fable_u8 g_FableThreadedFileVTable_0129A158;

extern "C" void FABLE_FASTCALL
FableThreadedFileBaseConstruct_0098DFD0(void* file, void*);
extern "C" void FABLE_FASTCALL
FableThreadedFilePathConstruct_0098DFD0(void* pathname, void*);

// CThreadedFile::CThreadedFile() @ 0x0098DFD0.
__declspec(naked) CThreadedFile::CThreadedFile()
{
    __asm
    {
        push esi
        mov esi, ecx
        call FableThreadedFileBaseConstruct_0098DFD0
        lea ecx, dword ptr [esi + 0Ch]
        mov dword ptr [esi], offset g_FableThreadedFileVTable_0129A158
        call FableThreadedFilePathConstruct_0098DFD0
        xor al, al
        mov byte ptr [esi + 18h], al
        mov byte ptr [esi + 19h], al
        mov dword ptr [esi + 8], -1
        mov eax, esi
        pop esi
        ret
    }
}
