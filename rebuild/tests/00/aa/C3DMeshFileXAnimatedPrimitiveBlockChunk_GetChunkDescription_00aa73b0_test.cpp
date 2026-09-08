#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileXAnimatedPrimitiveBlockChunkDescription[]="C3DMeshFileXAnimatedPrimitiveBlockChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileXAnimatedPrimitiveBlockChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileXAnimatedPrimitiveBlockChunkDescription))return 1;
 printf("C3DMESHFILEXANIMATEDPRIMITIVEBLOCKCHUNK_DESCRIPTION PASS\n");return 0;}