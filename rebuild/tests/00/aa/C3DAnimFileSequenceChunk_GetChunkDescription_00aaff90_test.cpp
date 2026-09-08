#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DAnimFileSequenceChunkDescription[]="C3DAnimFileSequenceChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DAnimFileSequenceChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DAnimFileSequenceChunkDescription))return 1;
 printf("C3DANIMFILESEQUENCECHUNK_DESCRIPTION PASS\n");return 0;}