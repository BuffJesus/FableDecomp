#include "rebuild_abi.h"
#include <cstdio>
struct CChunkedFileRawDataChunk { unsigned long IsUnknownType(); };
unsigned long CChunkedFileRawDataChunk::IsUnknownType(){ return 1; }
int main(){
    CChunkedFileRawDataChunk c;
    if(c.IsUnknownType()!=1){std::printf("BAD\n");return 1;}
    std::printf("ISUNK_OK\n"); return 0;
}