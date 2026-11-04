#include <stdio.h>
#include <stdlib.h>
#include "get_engine_def_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char __fastcall GetEngineDef(void*,void*,void*,unsigned,void*);
static unsigned char* arena;static FILE* output;
static unsigned mode,selected,indexValue;static unsigned* temporary;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned self=0,unsigned arg=0){
 unsigned h[]={kind,self,arg,temporary?*temporary:0};fwrite(h,4,4,output);fwrite(arena,1,2048,output);
}
extern "C" void* __fastcall LookupBoundary(void* self,void*,unsigned* result,void* name,unsigned index){
 if(self!=arena+0x400||name!=arena+0x40||index!=indexValue)exit(10);
 temporary=result;event(1,(unsigned)self,index);
 unsigned ptr=selected?(unsigned)(arena+selected*0x100):0;
 if(ptr)++*(unsigned*)(ptr+4);*result=ptr;return result;
}
extern "C" void __fastcall DeleteBoundary(void* self,void*){
 if(self!=arena+0x100&&self!=arena+0x200&&self!=arena+0x300)exit(11);
 event(2,(unsigned)self);
 if(mode==1)*temporary=0;
 else if(mode==2)*temporary=(unsigned)(arena+0x300);
 else if(mode==3){word(0)=(unsigned)(arena+0x300);word(0x304)=2;}
}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 word(0x1004)=(unsigned)&DeleteBoundary;
 unsigned p[4],cases=0;
 while(fread(p,4,4,input)==4){
  if(fread(arena,1,2048,input)!=2048)return 5;
  selected=p[0];mode=p[1];indexValue=p[2];temporary=0;
  unsigned result=GetEngineDef(arena+0x400,0,arena+0x40,indexValue,(void*)p[3]);temporary=0;event(3,0,result);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_GET_ENGINE_DEF_PASS cases=%u\n",cases);return 0;
}
