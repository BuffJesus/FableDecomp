#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kC3DAnimFileTimingEventChunkDescription[]="C3DAnimFileTimingEventChunk";
struct Chunk { CCharString GetChunkDescription() const; };
CCharString Chunk::GetChunkDescription() const{return CCharString(kC3DAnimFileTimingEventChunkDescription,-1);}
int main(){Chunk c;if(strcmp(c.GetChunkDescription().Data,kC3DAnimFileTimingEventChunkDescription))return 1;
 printf("C3DANIMFILETIMINGEVENTCHUNK_DESCRIPTION PASS\n");return 0;}