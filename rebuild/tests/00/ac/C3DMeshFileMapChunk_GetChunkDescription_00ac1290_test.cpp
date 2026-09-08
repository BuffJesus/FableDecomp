#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileMapChunkDescription[]="C3DMeshFileMapChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileMapChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileMapChunkDescription))return 1;
 printf("C3DMESHFILEMAPCHUNK_DESCRIPTION PASS\n");return 0;}