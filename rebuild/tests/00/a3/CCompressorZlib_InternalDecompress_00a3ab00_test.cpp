#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <string.h>
static int inits,resets,inflates,bad;static z_stream_s* saved;
static unsigned char input[17],output[29];
void* __cdecl CCompressorZlib::Alloc(void*,unsigned int,unsigned int){return 0;}
void __cdecl CCompressorZlib::Free(void*,void*){}
int __cdecl z_inflateInit_(z_stream_s* s,const char* ver,int size){
 ++inits;saved=s;if(strcmp(ver,"1.2.1") || size!=56 || !s->zalloc || !s->zfree)bad=1;return -1;
}
int __cdecl z_inflateReset(z_stream_s* s){++resets;if(s!=saved)bad=2;return -2;}
int __cdecl z_inflate(z_stream_s* s,int flush){
 ++inflates;if(s!=saved || flush!=4 || s->next_in!=input || s->avail_in!=17 || s->next_out!=output || s->avail_out!=29)bad=3;
 s->total_out=12+inflates;return -3;
}
int main(){
 double raw[2][sizeof(CCompressorZlib)/8];memset(raw,0xa5,sizeof(raw));
 CCompressorZlib* a=reinterpret_cast<CCompressorZlib*>(raw[0]);CCompressorZlib* b=reinterpret_cast<CCompressorZlib*>(raw[1]);
 if(a->InternalDecompress(0,0,0,0)!=0 || inits || resets || inflates)return 1;
 if(a->InternalDecompress(input,17,output,29)!=13 || inits!=1 || resets)return 2;
 if(b->InternalDecompress(input,17,output,29)!=14 || inits!=1 || resets!=1)return 3;
 if(a->InternalDecompress(0,0,0,0)!=0 || inflates!=2 || bad)return 4;
 for(unsigned i=0;i<sizeof(raw);++i)if(reinterpret_cast<unsigned char*>(raw)[i]!=0xa5)return 5;
 puts("INTERNAL_PASS");return 0;
}