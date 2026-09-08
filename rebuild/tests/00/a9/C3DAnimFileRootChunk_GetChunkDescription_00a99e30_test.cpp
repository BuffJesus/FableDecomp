#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DAnimFileRootChunkDescription[]="C3DAnimFileRootChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DAnimFileRootChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DAnimFileRootChunkDescription))return 1;
 printf("C3DANIMFILEROOTCHUNK_DESCRIPTION PASS\n");return 0;}