#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DAnimFileObjectChunkDescription[]="C3DAnimFileObjectChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DAnimFileObjectChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DAnimFileObjectChunkDescription))return 1;
 printf("C3DANIMFILEOBJECTCHUNK_DESCRIPTION PASS\n");return 0;}