#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_data_banks.h"
static union {double align;char bytes[0x190];} memory;
static CStreamingFontDataBank* owner;
static CCPPointerInfo blocks[3]={CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0)};
static const CCharString* liveString;static unsigned long indexValue;static int step,mode,releases,deletes;static void* freed[2];
static char objects[32],symbols[64];static CBankFileAsyncEntry* incomingData;static CCPPointerInfo* incomingInfo;
static void __fastcall release(void* p){++releases;if(p==objects){if(mode==1)owner->streamingEntry.Info=&blocks[2];if(mode==2)owner->streamingEntry.Info=0;}}
void __cdecl operator delete(void* p){if(owner){if(deletes>=2)abort();freed[deletes++]=p;}else free(p);}
CCharString::CCharString(const char* p,long n){if(step++!=0||strcmp(p,"GlyphData")||n!=-1)abort();storage_=(CCharStringData*)p;liveString=this;}
CCharString::~CCharString(){if(step++!=2)abort();storage_=0;}
unsigned long CBankFile::FindIndexBySymbol(const CCharString& s) const {if(this!=owner||step++!=1||&s!=liveString)abort();return indexValue;}
template<> CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(CBankFileAsyncEntry* p):Data(p),Info(incomingInfo){}
CCountedPointer<CBankFileAsyncEntry> CBankFileAsync::GetAsyncEntry(unsigned long i){if(this!=owner||step++!=3||i!=indexValue)abort();return CCountedPointer<CBankFileAsyncEntry>(incomingData);}
template<> CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer& r):Data(r.Data),Info(r.Info){if(Info)++Info->RefCount;}
int main(){unsigned long counts[]={0,0,1,2,0xffffffff};int cases=0;
for(int a=0;a<5;++a)for(int b=0;b<5;++b)for(int alias=0;alias<2;++alias)for(int m=0;m<3;++m)for(int ix=0;ix<3;++ix){
 memset(&memory,0,sizeof(memory));owner=(CStreamingFontDataBank*)&memory;mode=m;step=releases=deletes=0;indexValue=ix==0?0:ix==1?19:0xffffffff;
 for(int i=0;i<3;++i){blocks[i].RefCount=i==0?counts[a]:i==1?counts[b]:7;blocks[i].DeleteFunc=release;blocks[i].Data=objects+i*8;}
 CCPPointerInfo* old=a?&blocks[0]:0;incomingInfo=alias?old:b?&blocks[1]:0;incomingData=(CBankFileAsyncEntry*)(objects+24);
 owner->streamingEntry.Data=(CBankFileAsyncEntry*)objects;owner->streamingEntry.Info=old;
 bool changed=old!=incomingInfo;unsigned long oldExpected=counts[a]-(changed&&a?1:0);unsigned long newExpected=alias?oldExpected:counts[b];if(incomingInfo){if(changed)++newExpected;--newExpected;}
 int oldRelease=changed&&a==2;int temporaryRelease=incomingInfo&&newExpected==0;int expected=oldRelease+temporaryRelease;
 owner->InitStreamingEntry(*(CSymbolMap*)symbols);
 if(step!=4||owner->streamingEntry.Data!=(changed?incomingData:(CBankFileAsyncEntry*)objects)||owner->streamingEntry.Info!=(temporaryRelease&&alias&&m?m==1?&blocks[2]:0:incomingInfo)){printf("FAIL 1 a=%d b=%d alias=%d mode=%d ix=%d step=%d releases=%d deletes=%d\n",a,b,alias,m,ix,step,releases,deletes);return 1;}
 if(blocks[0].RefCount!=(alias&&incomingInfo?newExpected:oldExpected)||blocks[1].RefCount!=(!alias&&incomingInfo?newExpected:counts[b])){printf("FAIL 2 a=%d b=%d alias=%d mode=%d ix=%d step=%d releases=%d deletes=%d\n",a,b,alias,m,ix,step,releases,deletes);return 2;}
 if(releases!=expected||deletes!=expected){printf("FAIL 3 a=%d b=%d alias=%d mode=%d ix=%d step=%d releases=%d deletes=%d\n",a,b,alias,m,ix,step,releases,deletes);return 3;}
 if(oldRelease&&freed[0]!=(m==1?&blocks[2]:m==2?0:old)){printf("FAIL 4 a=%d b=%d alias=%d mode=%d ix=%d step=%d releases=%d deletes=%d\n",a,b,alias,m,ix,step,releases,deletes);return 4;}
 if(temporaryRelease&&freed[deletes-1]!=incomingInfo){printf("FAIL 5 a=%d b=%d alias=%d mode=%d ix=%d step=%d releases=%d deletes=%d\n",a,b,alias,m,ix,step,releases,deletes);return 5;}
 owner=0;++cases;
}printf("FONT_STREAMING_ENTRY_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
