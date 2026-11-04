#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_persistent_aliases.h"
#include "primitive_persistent_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,eventIndex);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Persistent(void*,void*,void*,void*,unsigned,unsigned);
extern "C" void *TypeGlobal,*TwoDGlobal;
static unsigned char *arena,*working,initial[4096],modelArena[4096];static unsigned p[8],cases,eventIndex,frameCount,currentType,current2D;static bool model;static FILE* output;
struct Frame{unsigned kind,self,a,b;unsigned char state[4096];unsigned type,twoD;}frames[16];
static unsigned& W(unsigned off){return *(unsigned*)(working+off);}
static unsigned P(unsigned off){return (unsigned)(arena+off);}
static void Event(unsigned kind,unsigned self,unsigned a=0,unsigned b=0){
 if(model){REQUIRE(frameCount<16);Frame&f=frames[frameCount++];f.kind=kind;f.self=self;f.a=a;f.b=b;memcpy(f.state,working,4096);f.type=currentType;f.twoD=current2D;}
 else {REQUIRE(eventIndex<frameCount);Frame&f=frames[eventIndex++];REQUIRE(f.kind==kind&&f.self==self&&f.a==a&&f.b==b&&!memcmp(f.state,arena,4096)&&(unsigned)TypeGlobal==f.type&&(unsigned)TwoDGlobal==f.twoD);fwrite(arena,1,4096,output);fwrite(&TypeGlobal,4,1,output);fwrite(&TwoDGlobal,4,1,output);currentType=(unsigned)TypeGlobal;current2D=(unsigned)TwoDGlobal;}
 unsigned off=self-(unsigned)arena;
 if(kind==1){if(p[4]==1)W(0x24)=0;if(p[4]==2)W(0x24)=P(0x180);if(p[4]==3)W(off+4)=2;if(p[4]==4)currentType=P(0x600);if(p[4]==5)W(off+4)=0;}
 if(kind==2){++W(off+12);if(p[4]==5)W(0x24)=P(0x180);}
 if(kind==3){if(p[4]==1)W(0x24)=P(0x180);if(p[4]==2)W(0x24)=P(0x100);if(p[4]==3){W(0x40)=(p[0]+1)%47;currentType=P(0x600);}if(p[4]==4)current2D=P(0x940);if(p[4]==5)W(0x24)=P(0x280);}
 if(kind==4){if(p[4]==1)W(0x24)=0;if(p[4]==2)W(0x24)=P(0x180);if(p[4]==3){working[off+0x24]^=0x80;working[off+0x34]^=0xfd;}if(p[4]==4){currentType=P(0x600);current2D=P(0x940);}if(p[4]==5){W(0x40)=(p[0]+3)%47;W(off+12)^=0x12345678;}}
 if(!model){TypeGlobal=(void*)currentType;TwoDGlobal=(void*)current2D;}
}
static void ClearModel(){unsigned q=W(0x24);if(q&&W(q-(unsigned)arena+4)==1)Event(1,q,P(0x20));q=W(0x24);if(q){if(--W(q-(unsigned)arena+4)==0)Event(2,q);W(0x24)=0;}W(0x24)=0;}
static void Reference(){
 unsigned type=W(0x40),manager=W(currentType-(unsigned)arena+0x10+4*type),q=W(0x24);unsigned char enabled=(p[3]&0x20)?0:1;
 if(q&&W(q-(unsigned)arena+8)==type){Event(4,q,P(0x40),p[3]);unsigned off=q-(unsigned)arena;working[off+0x34]=(working[off+0x34]&0xfe)|enabled;if((signed char)working[off+0x24]>=0)Event(5,q,p[3]);Event(8,0);return;}
 if(q)ClearModel();
 if(manager){Event(3,manager,type);q=P(0x200);ClearModel();W(0x24)=q;working[0x234]=(working[0x234]&0xfd)|(p[5]<<1);Event(4,q,P(0x40),p[3]);working[0x234]=(working[0x234]&0xfe)|enabled;if(p[3]&0x40)Event(7,current2D,q);else Event(6,manager,q,p[3]);}
 Event(8,0);
}
static void __fastcall Notify(void*self,void*,void*h){Event(1,(unsigned)self,(unsigned)h);}
static void __fastcall Destroy(void*self,void*){Event(2,(unsigned)self);}
static void* __fastcall Factory(void*self,void*,unsigned type){Event(3,(unsigned)self,type);return arena+0x200;}
static void __fastcall Update(void*self,void*,void*v,unsigned flags){Event(4,(unsigned)self,(unsigned)v,flags);}
extern "C" void __fastcall SceneUpdate(void*self,unsigned flags){Event(5,(unsigned)self,flags);}
extern "C" void __fastcall ManagerAdd(void*self,void*,void*primitive,unsigned flags){Event(6,(unsigned)self,(unsigned)primitive,flags);}
extern "C" void __fastcall Add2D(void*self,void*,void*primitive){Event(7,(unsigned)self,(unsigned)primitive);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<5;++i){unsigned v;switch(retailFixups[i][1]){case 0x1436e84:v=(unsigned)&TypeGlobal;break;case 0x1436e7c:v=(unsigned)&TwoDGlobal;break;case 0xb91840:v=(unsigned)&SceneUpdate;break;case 0xbacdd0:v=(unsigned)&ManagerAdd;break;case 0xb4a670:v=(unsigned)&Add2D;break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 static void* primitiveTable[6];primitiveTable[1]=(void*)&Destroy;primitiveTable[3]=(void*)&Notify;primitiveTable[5]=(void*)&Update;static void* managerTable[2];managerTable[1]=(void*)&Factory;
 typedef void(__fastcall*Fn)(void*,void*,void*,void*,unsigned,unsigned);Fn fn=!strcmp(argv[3],"retail")?(Fn)code:&Persistent;
 while(fread(p,4,8,in)==8){working=arena;memset(arena,0x95,4096);W(0x24)=p[1]?P(0x100):0;W(0x40)=p[0];for(unsigned off=0x100;off<=0x280;off+=0x80){W(off)=(unsigned)primitiveTable;W(off+4)=off==0x100?p[6]:off==0x180?1:2;W(off+8)=p[0];working[off+0x24]=(unsigned char)p[7];working[off+0x34]=(unsigned char)(0xa4+p[4]);}W(0x108)=p[1]==2?(p[0]+1)%47:p[0];W(0x204)=1;W(0x800)=W(0x840)=(unsigned)managerTable;for(unsigned typ=0;typ<47;++typ){W(0x410+4*typ)=p[2]?P(0x800):0;W(0x610+4*typ)=P(0x840);}memcpy(initial,arena,4096);memcpy(modelArena,arena,4096);
  currentType=P(0x400);current2D=P(0x900);working=modelArena;model=true;frameCount=eventIndex=0;Reference();
  memcpy(arena,initial,4096);working=arena;TypeGlobal=(void*)P(0x400);TwoDGlobal=(void*)P(0x900);model=false;eventIndex=0;fn(arena,0,arena+0x20,arena+0x40,p[3],p[5]);Event(8,0);REQUIRE(eventIndex==frameCount);++cases;
 }fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_PERSISTENT_PASS cases=%u\n",cases);return 0;
}
