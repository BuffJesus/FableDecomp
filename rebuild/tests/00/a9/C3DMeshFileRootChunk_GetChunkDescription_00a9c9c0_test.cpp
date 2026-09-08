#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileRootChunkDescription[]="C3DMeshFileRootChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileRootChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileRootChunkDescription))return 1;
 printf("C3DMESHFILEROOTCHUNK_DESCRIPTION PASS\n");return 0;}