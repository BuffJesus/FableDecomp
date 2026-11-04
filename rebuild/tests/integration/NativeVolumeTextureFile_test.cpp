#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volume_texture_file_retail.h"
#include "volume_texture_file_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall SaveFile(void*,void*,void*);
extern "C" void __fastcall LoadFile(void*,void*,void*);
static unsigned arena[4096],initial[4096],images[4][4096],mode,step,count,snapshots,location,profile,mutation,operation,phase,owner,file;
static unsigned* active;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void snapshot(){if(!mode){require(count<4);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(){word(ptr(0x2000),0)=phase;word(ptr(0x2000),4)=operation;word(ptr(0x2000),8)=file;memcpy((void*)ptr(0x2020),active,(operation?9:3)*4);snapshot();}
static void* constructor(void* self,void* input,unsigned which){require(!active&&!phase&&which==operation&&(unsigned)input==file);active=(unsigned*)self;for(unsigned i=0;i<(operation?9:3);++i)active[i]=pattern(profile+i);phase=1;if(mutation&1){word(owner,0)^=pattern(profile+3);word(owner,4)=pattern(profile+4);}event();return self;}
static void serializer(void* self,void* stream,unsigned which){require(active==(unsigned*)stream&&phase==1&&(unsigned)self==owner&&which==operation);phase=2;if(mutation&2){for(unsigned i=0;i<(operation?9:3);++i)active[i]^=pattern(profile+i+5);word(owner,8)=pattern(profile+2);}event();}
static void destructor(void* self,unsigned which){require(active==(unsigned*)self&&phase==2&&which==operation);phase=3;active[0]=pattern(profile+7);word(owner,12)^=pattern(profile+6);event();active=0;}
extern "C" void* __fastcall OutputCtor(void* self,void*,void* input){return constructor(self,input,0);}
extern "C" void __fastcall OutputDtor(void* self,void*){destructor(self,0);}
extern "C" void __fastcall SaveStream(void* self,void*,void* stream){serializer(self,stream,0);}
extern "C" void* __fastcall InputCtor(void* self,void*,void* input,unsigned long buffer){require(buffer==0x4000);word(ptr(0x2000),12)=buffer;return constructor(self,input,1);}
extern "C" void __fastcall InputDtor(void* self,void*){destructor(self,1);}
extern "C" void __fastcall LoadStream(void* self,void*,void* stream){serializer(self,stream,1);}
static void model(){unsigned stream[9];if(operation)InputCtor(stream,0,(void*)file,0x4000);else OutputCtor(stream,0,(void*)file);serializer((void*)owner,stream,operation);destructor(stream,operation);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[6]={(unsigned)OutputCtor,(unsigned)OutputDtor,(unsigned)SaveStream,(unsigned)InputCtor,(unsigned)InputDtor,(unsigned)LoadStream};for(unsigned f=0;f<sizeof(fixups)/sizeof(*fixups);++f){require(fixups[f].kind==20&&fixups[f].target<6);*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);}unsigned cases=0;
for(profile=0;profile<8;++profile)for(operation=0;operation<2;++operation)for(unsigned fc=0;fc<3;++fc)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;owner=ptr(0x100);file=fc==0?0:fc==1?1:0xffffffff;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<4;++i)word(owner,i*4)=pattern(profile+i);memset((void*)ptr(0x2000),0,80);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=phase=0;active=0;if(!mode)model();else{void(__fastcall* function)(void*,void*,void*)=mode==1?(void(__fastcall*)(void*,void*,void*))(code+(operation?64:0)):(operation?LoadFile:SaveFile);function((void*)owner,0,(void*)file);}require(!active&&phase==3);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_VOLUME_TEXTURE_FILE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
