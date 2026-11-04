#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_constructor_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructMap(void*,void*,const void*,long,long,long,long,long,unsigned,unsigned);
extern "C" void __fastcall DefaultString(void*);
extern "C" void __fastcall AssignString(void*,void*,const void*);
extern "C" void __fastcall DestroyString(void*);
#pragma comment(linker,"/alternatename:@ConstructMap@40=??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z")
#pragma comment(linker,"/alternatename:@DefaultString@4=??0CCharString@@QAE@XZ")
#pragma comment(linker,"/alternatename:@AssignString@12=??4CCharString@@QAEAAV0@ABV0@@Z")
#pragma comment(linker,"/alternatename:@DestroyString@4=??1CCharString@@QAE@XZ")
static unsigned char* arena;static FILE* output;static unsigned mode;
static void record(unsigned kind,unsigned arg){unsigned h[]={kind,arg};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
CCharString::CCharString(){if((unsigned char*)this!=arena+0x1c)exit(10);record(1,0);*(unsigned*)this=0x1111;if(mode==1)memset(arena,0x5a,0x68);}
CCharString& CCharString::operator=(const CCharString& other){if((unsigned char*)this!=arena+0x1c)exit(11);record(3,*(unsigned*)&other);*(unsigned*)this=*(unsigned*)&other;if(mode==2)arena[0x64]=0xff;return *this;}
CCharString::~CCharString(){record(4,*(unsigned*)this);}
extern "C" void* __cdecl MapAllocate(unsigned size){if(size!=36)exit(12);record(2,size);memset(arena+0x400,0xcd,size);if(mode==3)*(unsigned*)(arena+0x1c)=0xabcd;return arena+0x400;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x99e4b0:v=(unsigned)DefaultString;break;case 0x99efb0:v=(unsigned)AssignString;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0xbfea0e:v=(unsigned)MapAllocate;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void* (__fastcall *Call)(void*,void*,const void*,long,long,long,long,long,unsigned,unsigned);Call run=argv[3][0]=='r'?(Call)code:ConstructMap;
 unsigned p[9],cases=0;while(fread(p,4,9,in)==9){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];void* result=run(arena,0,(void*)p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8]);if(result!=arena)return 6;record(5,(unsigned)result);++cases;}
 fclose(in);fclose(output);printf("NATIVE_MAP_CONSTRUCTOR_PASS cases=%u\n",cases);return 0;
}
