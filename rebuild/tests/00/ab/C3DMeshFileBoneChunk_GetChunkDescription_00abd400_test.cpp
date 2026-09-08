#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileBoneChunkDescription[]="C3DMeshFileBoneChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileBoneChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileBoneChunkDescription))return 1;
 printf("C3DMESHFILEBONECHUNK_DESCRIPTION PASS\n");return 0;}