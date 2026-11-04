#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_delete_aliases.h"
#include "map_delete_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,long);
extern "C" void* LandscapeGlobal;extern "C" void* DetailGlobal;
static unsigned char* arena;static FILE* output;static unsigned mode,index;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg,(unsigned)LandscapeGlobal,(unsigned)DetailGlobal};if(fwrite(h,4,5,output)!=5||fwrite(arena,1,8192,output)!=8192)exit(9);}
static void replaceArray(){unsigned bytes=*(unsigned*)(arena+0x24)-*(unsigned*)(arena+0x20);*(void**)(arena+0x20)=arena+0x420;*(void**)(arena+0x24)=arena+0x420+bytes;}
extern "C" void __cdecl MapFree(void* p){record(1,p,0);memset(p,0xdd,12);if(mode==1||mode==5||mode==6)replaceArray();}
extern "C" void __fastcall LandscapeVisibility(void* self,void*,bool visible){record(2,self,visible);if(mode==2)*(void**)(arena+0x600+index*0x100+4)=arena+0x1200;}
extern "C" void __fastcall DetailVisibility(void* self,void*,bool visible){record(3,self,visible);if(mode==3)LandscapeGlobal=arena+0x1300;}
extern "C" void* __fastcall LandscapeDtor(void* self,void*,unsigned flags){if(flags!=1)exit(15);record(4,self,flags);if(mode==4){*(void**)(arena+0x400+index*4)=arena+0x900+index*0x100;DetailGlobal=arena+0x1400;}return self;}
extern "C" unsigned BaseTable[],QuadTable[];
extern "C" void* __fastcall QuadChildDelete(void* self,void*,unsigned flags){record(16,self,flags);if(flags!=1)exit(16);return self;}
extern "C" void* __fastcall QuadCollectionDelete(void* self,void*,unsigned flags){record(15,self,flags);if(flags!=1)exit(17);return self;}
extern "C" void __fastcall FreeVirtual(void*,void*,void*){exit(18);}
extern "C" void __fastcall CloseLandscape(void* self){record(9,self,0);}
extern "C" void __fastcall CloseDetail(void* self){record(10,self,0);}
extern "C" void __fastcall StringDtor(void* self){record(13,self,*(unsigned*)self);}
extern "C" void __fastcall DeleteData(void* self){record(14,self,0);if(mode==7)*(void**)((unsigned char*)self+0x40)=arena+0x1680;}
void __cdecl operator delete(void* p){if(output){record(7,p,0);if(mode==7){*(void**)(arena+0x20)=arena+0x420;*(void**)(arena+0x24)=arena+0x42c;}}}
extern "C" void __fastcall Fill(void*,void*,void*,unsigned,void*){exit(10);}
int main(int argc,char** argv){
 if(argc!=4||LandscapeGlobal||DetailGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],target=retailFixups[i][1],v=0;
  switch(target){case 0xbdbff0:v=(unsigned)(code+352);break;case 0xbfea14:v=(unsigned)MapFree;break;case 0xbdc3c0:v=(unsigned)LandscapeVisibility;break;case 0xbde160:v=(unsigned)DetailVisibility;break;case 0xb3ee80:v=(unsigned)(code+384);break;case 0x1436ea8:v=(unsigned)&LandscapeGlobal;break;case 0xb67180:v=(unsigned)(code+2176);break;case 0x1436e88:v=(unsigned)&DetailGlobal;break;case 0xb59840:v=(unsigned)(code+2240);break;case 0xbded60:v=(unsigned)(code+2304);break;case 0xbddfb0:v=(unsigned)(code+2400);break;case 0xbdda50:v=(unsigned)(code+2528);break;case 0xbe0400:v=(unsigned)(code+2592);break;case 0xbe08e0:v=(unsigned)(code+2656);break;case 0x129b860:v=(unsigned)BaseTable;break;case 0x12a7df0:v=(unsigned)QuadTable;break;case 0xb42a90:v=(unsigned)(code+544);break;case 0xb3ef40:v=(unsigned)(code+704);break;case 0xbe05d0:v=(unsigned)(code+832);break;case 0xb443b0:v=(unsigned)(code+1088);break;case 0xb441f0:v=(unsigned)(code+1216);break;case 0x42a34e:v=(unsigned)(code+1344);break;case 0x41c7e0:v=(unsigned)(code+1920);break;case 0x41c800:v=(unsigned)(code+1952);break;case 0x42951b:v=(unsigned)(code+1984);break;case 0x42955b:v=(unsigned)(code+2048);break;case 0x99eae0:v=(unsigned)StringDtor;break;case 0xbdc4f0:v=(unsigned)CloseLandscape;break;case 0xbddd50:v=(unsigned)CloseDetail;break;case 0xbfe9bc:v=(unsigned)&operator delete;break;case 0xbfeae6:v=(unsigned)memmove;break;case 0xb43f80:v=(unsigned)Fill;break;default:return 11;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Call)(void*,void*,long);Call entry=argv[3][0]=='r'?(Call)code:Entry;unsigned p[4],cases=0;
 while(fread(p,4,4,in)==4){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];index=p[1];LandscapeGlobal=p[2]?arena+0x1300:0;DetailGlobal=p[3]?arena+0x1400:0;
  *(void**)(arena+0x1e20)=(void*)QuadCollectionDelete;*(void**)(arena+0x1e40)=(void*)QuadChildDelete;*(void**)(arena+0x1e00)=(void*)LandscapeDtor;*(void**)(arena+0x1744)=(void*)DeleteData;*(void**)(arena+0x1784)=(void*)DeleteData;*(void**)(arena+0x1604)=(void*)DeleteData;*(void**)(arena+0x1644)=(void*)DeleteData;*(void**)(arena+0x1684)=(void*)DeleteData;entry(arena,0,index);record(8,0,0);entry(arena,0,index);record(8,0,1);++cases;
 }
 fclose(in);fclose(output);output=0;printf("NATIVE_MAP_DELETE_PASS cases=%u\n",cases);return 0;
}
