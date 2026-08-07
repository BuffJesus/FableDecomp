#include "rebuild_abi.h"
// CWADFile::Close @ 0x00733210
// mov [ecx+0xc],0xfffffcf7; mov [ecx+0x10],0xfffffc19; ret
// Reset two handle/offset fields to their sentinel values.
struct CWADFile { char pad00[0xc]; long f0c; long f10; void Close(); };
void CWADFile::Close()
{
    f0c = (long)0xfffffcf7;
    f10 = (long)0xfffffc19;
}