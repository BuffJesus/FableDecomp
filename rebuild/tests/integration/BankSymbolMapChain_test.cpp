#include "fable_game_text_bank.h"
#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[]="";
static CBankFile* active;static unsigned long mode;static int live,clears,updates,added,enumerations,failed;
static unsigned long expected[16],types[16],expectedCount;static const char* names[16];
static char trace[512];static unsigned traceSize;
static void event(char c){trace[traceSize++]=c;}
CCharString::CCharString(const char* s,long n){if(n!=-1)abort();storage_=(CCharStringData*)malloc(4);*(const char**)storage_=s;++live;event('s');}
CCharString::CCharString(const CCharString& s){storage_=(CCharStringData*)malloc(4);*(const char**)storage_=*(const char**)s.storage_;++live;event('s');}
CCharString::~CCharString(){free(storage_);--live;event('d');}
void CSymbolMap::Clear(){++clears;event('c');if(mode==1)active->Size=0;}
void CSymbolMap::UpdateDirty()const{++updates;event('u');}
void CSymbolMap::AddSymbolDirty(const CCharString& group,const CCharString& name,long index){
 event('a');if(added>=expectedCount||index!=expected[added]||strcmp(**(const char* const* const*)&name,names[index])||strcmp(**(const char* const* const*)&group,types[index]&1?"odd":"even"))failed=1;
 ++added;if(mode==3)active->Size=index+1;
}
struct EnumerationBoundary{virtual CCharString Get(unsigned long type)const{event('e');++enumerations;if(mode==2)active->Size=0;return CCharString(type&1?"odd":"even");}};
static void trap(){abort();}
int main(){
 static union{double align;char data[0x110];} bankMem;static union{double align;char data[0x40];} mapMem;
 static unsigned long symbols[16],valid[16],dataTypes[16];static CBankFile::CRuntimeData runtime[16];
 void* vtable[20];for(int z=0;z<20;++z)vtable[z]=(void*)&trap;EnumerationBoundary boundary;vtable[19]=(*(void***)&boundary)[0];
 CBankFile* bank=(CBankFile*)&bankMem;CSymbolMap* map=(CSymbolMap*)&mapMem;active=bank;int cases=0;
 static const char* keys[]={"zero","one","[two]","three[","","five","six","seven"};
 for(unsigned n=0;n<=8;++n)for(unsigned packed=0;packed<4;++packed)for(unsigned missing=0;missing<2;++missing)for(unsigned append=0;append<2;++append)for(mode=0;mode<4;++mode){
  memset(&bankMem,0,sizeof(bankMem));*(void***)bank=vtable;bank->Size=n;
  *(unsigned long**)&bank->Symbols=symbols;((unsigned long**)&bank->Symbols)[1]=symbols+(missing?0:n);
  *(CBankFile::CRuntimeData**)&bank->RuntimeData=runtime;
  bank->PackedValid.Bits=32;bank->PackedDataType.Bits=32;bank->PackedValid.Size=packed&1;bank->PackedValid.PackedInts=valid;
  bank->PackedDataType.Size=packed&2;bank->PackedDataType.PackedInts=dataTypes;
  for(unsigned i=0;i<8;++i){symbols[i]=(unsigned long)&keys[i];runtime[i].Valid=i%3!=0;runtime[i].DataType=(unsigned char)(i*13);if(i){valid[i-1]=i%4!=0;dataTypes[i-1]=0xffffff00+i;}names[i]=missing?"(Unknown)":keys[i];types[i]=(packed&2)&&i?dataTypes[i-1]:runtime[i].DataType;}
  // Index zero is never queried; do not read its nonexistent packed predecessor.
  expectedCount=0;if(!(mode==1&&!append))for(i=1;i<n;++i){if((packed&1?valid[i-1]!=0:runtime[i].Valid)&&!strchr(names[i],'[')){expected[expectedCount++]=i;if(mode==2||mode==3)break;}}
  live=clears=updates=added=enumerations=failed=0;traceSize=0;bank->CreateSymbolMap(*map,append!=0);
  if(failed||live||clears!=!append||updates!=1||added!=expectedCount||enumerations!=added||trace[traceSize-1]!='u')return 1;
  ++cases;
 }
 printf("BANK_SYMBOL_MAP_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;
}
