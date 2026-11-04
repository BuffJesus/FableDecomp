#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#include "resource_loading_process_retail.h"
#include "resource_loading_process_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ProcessQueue(void*,void*,bool,double);
extern "C" bool AbortFlag;
#pragma comment(linker,"/alternatename:_AbortFlag=?ResourceAbortingEnabled@CResourceBank@@0_NA")
static unsigned arena[4096],initial[4096],images[32][4096],flags[32],mode,step,count,snapshots,location,mask,mutation,timeProfile,calls;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b){fprintf(stderr,"loading process failure mode=%u step=%u mutation=%u mask=%u time=%u\n",mode,step,mutation,mask,timeProfile);abort();}}
static void snapshot(){if(!mode){require(count<32);memcpy(images[count],arena,sizeof(arena));flags[count++]=AbortFlag;}else{require(step<count&&!memcmp(images[step],arena,sizeof(arena))&&flags[step]==(unsigned)AbortFlag);++step;++snapshots;}}
static void event(unsigned node,unsigned kind,unsigned value){require(++calls<30);word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=kind;word(ptr(0x2000),8)=node;word(ptr(0x2000),12)=value;if(mutation==1){++word(ptr(0x100),44);AbortFlag=!AbortFlag;}if((kind==1||kind==2)&&mutation==2)word(node,20)^=0xdeadbeef;if((kind==3||kind==4)&&mutation>=3)word(ptr(0x100),20)=0;if((kind==1||kind==2)&&mutation==4)word(node,16)=0;if((kind==1||kind==2)&&mutation==5)word(node,12)=ptr(0x1800);snapshot();}
static unsigned index(unsigned node){require(node>=ptr(0x400)&&node<=ptr(0x600)&&!(node-ptr(0x400)&255));return (node-ptr(0x400))/256;}
class Boundaries{public:bool CanAbort(long)const;bool IsLoaded()const;void Loaded();void Aborted();};
bool Boundaries::CanAbort(long age)const{unsigned node=(unsigned)this,i=index(node);require((unsigned)age==word(ptr(0x100),44)-word(node,24));event(node,1,(unsigned)age);return (mask&(1<<((i+1)%3)))!=0;}
bool Boundaries::IsLoaded()const{unsigned node=(unsigned)this,i=index(node);event(node,2,0);return (mask&(1<<i))!=0;}
void Boundaries::Loaded(){event((unsigned)this,3,0);}
void Boundaries::Aborted(){event((unsigned)this,4,0);}
static double real(unsigned long lo,unsigned long hi){unsigned words[2]={lo,hi};double d;memcpy(&d,words,8);return d;}
double __fastcall GFGetTime(){event(0,0,timeProfile);const double times[6]={0.0,2.0,1.0,0.0,0.0,0.0};return timeProfile<3?times[timeProfile]:timeProfile==3?real(1,0x7ff80000):timeProfile==4?real(0,0x7ff00000):real(0,0xfff00000);}
static void remove(unsigned list,unsigned node){word(node,8)=0;unsigned next=word(node,16);if(next){unsigned prev=word(node,12);if(word(prev,16)==node&&word(next,12)==node){word(prev,16)=next;word(word(node,16),12)=word(node,12);}word(node,12)=word(node,16)=0;}--word(list,32);word(list,36)-=word(node,20);}
static void model(bool bounded,double deadline){unsigned list=ptr(0x100),node=word(list,20);while(node!=list+4){if(!node)return;if(bounded&&GFGetTime()>deadline)return;unsigned next=word(node,16);if(AbortFlag&&((Boundaries*)node)->CanAbort(word(list,44)-word(node,24))){remove(list,node);((Boundaries*)node)->Aborted();next=word(list,20);}else if(((Boundaries*)node)->IsLoaded()){remove(list,node);((Boundaries*)node)->Loaded();next=word(list,20);}node=next;}}
int main(){require(AbortFlag);unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].kind==20?(unsigned)&GFGetTime:(unsigned)&AbortFlag;*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}unsigned table[10]={0};bool(Boundaries::*a)(long)const=&Boundaries::CanAbort;bool(Boundaries::*l)()const=&Boundaries::IsLoaded;void(Boundaries::*n)()=&Boundaries::Loaded;void(Boundaries::*b)()=&Boundaries::Aborted;memcpy(&table[6],&a,4);memcpy(&table[4],&l,4);memcpy(&table[5],&n,4);memcpy(&table[7],&b,4);const unsigned raw[3]={0,0x80000000,0xffffffff};unsigned cases=0;
for(unsigned nodes=0;nodes<4;++nodes)for(unsigned enabled=0;enabled<2;++enabled)for(unsigned bounded=0;bounded<2;++bounded)for(mask=0;mask<8;++mask)for(mutation=0;mutation<6;++mutation)for(timeProfile=0;timeProfile<6;++timeProfile)for(unsigned age=0;age<3;++age)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));unsigned list=ptr(0x100),head=list+4;word(head,16)=nodes?ptr(0x400):head;word(head,12)=nodes?ptr(0x400+(nodes-1)*256):head;word(list,32)=raw[age];word(list,36)=raw[(age+1)%3];word(list,44)=raw[(age+2)%3];for(unsigned i=0;i<3;++i){unsigned node=ptr(0x400+i*256);word(node,0)=(unsigned)table;word(node,8)=list;word(node,12)=i?ptr(0x400+(i-1)*256):head;word(node,16)=i+1<nodes?ptr(0x400+(i+1)*256):head;word(node,20)=raw[(i+age)%3];word(node,24)=raw[(i+age+1)%3];}word(ptr(0x1800),16)=0;memset((void*)ptr(0x2000),0,16);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));AbortFlag=enabled!=0;step=calls=0;if(!mode)model(bounded!=0,1.0);else(mode==1?((void(__fastcall*)(void*,void*,bool,double))code):ProcessQueue)((void*)list,0,bounded!=0,1.0);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_RESOURCE_LOADING_PROCESS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
