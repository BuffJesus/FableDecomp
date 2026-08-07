#include "rebuild_abi.h"
#include <cstdio>
static int g_base=0;
void *FableVtbl_CChunkedFileChunk_c10a10[4];
struct CChunkedFileChunk { void *vptr; void Dtor(); };
extern "C" void FABLE_FASTCALL FableBaseDtor_c10a10(CChunkedFileChunk *self){ g_base=1; }
void CChunkedFileChunk::Dtor(){ vptr=(void*)&FableVtbl_CChunkedFileChunk_c10a10; FableBaseDtor_c10a10(this); }
int main(){ CChunkedFileChunk o; o.Dtor();
 if(o.vptr!=(void*)&FableVtbl_CChunkedFileChunk_c10a10||g_base!=1){std::printf("B1\n");return 1;}
 std::printf("DT_c10a10_OK\n"); return 0; }