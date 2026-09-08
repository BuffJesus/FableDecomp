#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileUniqueVerticesChunkDescription[]="C3DMeshFileUniqueVerticesChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileUniqueVerticesChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileUniqueVerticesChunkDescription))return 1;
 printf("C3DMESHFILEUNIQUEVERTICESCHUNK_DESCRIPTION PASS\n");return 0;}