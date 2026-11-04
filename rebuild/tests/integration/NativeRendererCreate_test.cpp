#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer_create_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Landscape(void*,void*,void*,long);
extern "C" void __fastcall Detail(void*,void*,void*);
#pragma comment(linker,"/alternatename:@Landscape@16=?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z")
#pragma comment(linker,"/alternatename:@Detail@12=?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z")
#pragma comment(linker,"/alternatename:??0CEngineLandscapeMap@@QAE@PBVCEngineMap@@J@Z=@ConstructLandscape@16")
#pragma comment(linker,"/alternatename:??0CLocalDetailCacheMap@@QAE@PAVCEngineMap@@@Z=@ConstructDetail@12")
static unsigned char* arena;static FILE* output;static unsigned mode,nullmask,allocation;
static void record(unsigned kind,unsigned arg){unsigned h[]={kind,arg};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
void* __cdecl operator new(unsigned size){if(allocation>3||(size!=0x60&&size!=0x70))exit(10);record(1,size);if(mode==1){*(void**)arena=arena+0x700;*(void**)(arena+4)=arena+0x780;}unsigned bit=1<<(size==0x70);unsigned index=allocation++;return nullmask&bit?0:arena+0x100+index*0x100;}
extern "C" void* __fastcall ConstructLandscape(void* self,void*,const void* map,long flags){if(map!=arena)exit(11);record(2,flags);memset(self,0x61,0x60);if(mode==2)*(void**)arena=arena+0x700;if(mode==3)*(void**)(arena+4)=0;return self;}
extern "C" void* __fastcall ConstructDetail(void* self,void*,void* map){if(map!=arena)exit(12);record(3,(unsigned)self);memset(self,0x71,0x70);if(mode==2)*(void**)(arena+4)=arena+0x780;if(mode==3)*(void**)arena=0;return self;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0xbfea1a:v=(unsigned)&operator new;break;case 0xbdd2f0:v=(unsigned)ConstructLandscape;break;case 0xbdefc0:v=(unsigned)ConstructDetail;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Land)(void*,void*,void*,long);typedef void (__fastcall *Det)(void*,void*,void*);Land land=argv[3][0]=='r'?(Land)code:Landscape;Det det=argv[3][0]=='r'?(Det)(code+64):Detail;
 unsigned p[5],cases=0;while(fread(p,4,5,in)==5){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];nullmask=p[1];allocation=0;
  for(unsigned j=0;j<2;++j){if(p[4]){det((void*)p[3],0,arena);land((void*)p[3],0,arena,p[2]);}else{land((void*)p[3],0,arena,p[2]);det((void*)p[3],0,arena);}record(4+j,0);}++cases;}
 fclose(in);fclose(output);printf("NATIVE_RENDERER_CREATE_PASS cases=%u\n",cases);return 0;
}
