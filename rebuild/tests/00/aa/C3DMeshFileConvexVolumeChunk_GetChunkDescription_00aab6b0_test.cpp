#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DMeshFileConvexVolumeChunkDescription[]="C3DMeshFileConvexVolumeChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DMeshFileConvexVolumeChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DMeshFileConvexVolumeChunkDescription))return 1;
 printf("C3DMESHFILECONVEXVOLUMECHUNK_DESCRIPTION PASS\n");return 0;}