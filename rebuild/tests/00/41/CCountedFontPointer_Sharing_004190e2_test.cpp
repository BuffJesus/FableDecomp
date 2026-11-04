#include "fable_font_bank.h"
#include "fable_counted_pointer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// The fixture executes the recovered release algorithm for both pointer types.
#include "../src/compiled/00/40/CCountedPointerFontBank_ReleaseData_004057a0.cpp"
template void CCountedPointer<const CFontBank>::ReleaseData();
static unsigned failures,callbackMode,callbackCalls,deleteCalls;
static bool cleanup,freed[3];
static CCPPointerInfo* blocks[3];
static CCountedPointer<CFontBank>* destination;
static CCountedPointer<CFontBank>* source;
static void check(bool v){if(!v)++failures;}
void operator delete(void* p){
 if(!cleanup){++deleteCalls;for(unsigned i=0;i<3;++i)if(p==blocks[i])freed[i]=true;}
 free(p);
}
static void __fastcall ReleaseCallback(void* p){
 check(p==reinterpret_cast<void*>(0x3333));++callbackCalls;
 if(callbackMode==1)destination->Info=blocks[2];
 if(callbackMode==2){source->Data=reinterpret_cast<CFontBank*>(0x7777);source->Info=blocks[2];}
 if(callbackMode==3)destination->Info=0;
}
int main(){unsigned cases=0;
 const unsigned long counts[5]={0,0,1,2,0xffffffffUL};
 for(unsigned kind=0;kind<4;++kind)for(unsigned oldCase=0;oldCase<5;++oldCase)for(unsigned newCase=0;newCase<5;++newCase)for(unsigned alias=0;alias<3;++alias)for(callbackMode=0;callbackMode<4;++callbackMode){
  cleanup=false;memset(freed,0,sizeof(freed));callbackCalls=deleteCalls=0;
  for(unsigned i=0;i<3;++i)blocks[i]=new CCPPointerInfo(ReleaseCallback,reinterpret_cast<void*>(0x3333));
  blocks[0]->RefCount=counts[oldCase];blocks[1]->RefCount=counts[newCase];
  unsigned long lhs[2],rhs[2];destination=reinterpret_cast<CCountedPointer<CFontBank>*>(lhs);source=reinterpret_cast<CCountedPointer<CFontBank>*>(rhs);
  destination->Data=reinterpret_cast<CFontBank*>(0xaaaa);destination->Info=oldCase?blocks[0]:0;
  source->Data=reinterpret_cast<CFontBank*>(0xbbbb);source->Info=alias==1?destination->Info:newCase?blocks[1]:0;
  if(alias==2)source=destination;
  CCPPointerInfo* old=destination->Info;CCPPointerInfo* incoming=source->Info;CFontBank* incomingData=source->Data;
  bool changed=old!=incoming;bool released=changed && old && counts[oldCase]==1;
  unsigned long incomingCount=incoming?incoming->RefCount:0;
  if(kind==0)check(&(*destination=*source)==destination);
  else if(kind==1)check(&(*reinterpret_cast<CCountedPointer<const CFontBank>*>(destination)=*source)==reinterpret_cast<CCountedPointer<const CFontBank>*>(destination));
  else if(kind==2)destination->ShareData(source->Data,source->Info);
  else reinterpret_cast<CCountedPointer<const CFontBank>*>(destination)->ShareData(source->Data,source->Info);
  check(destination->Info==incoming && destination->Data==(changed?incomingData:reinterpret_cast<CFontBank*>(0xaaaa)));
  check(callbackCalls==(released?1:0) && deleteCalls==(released?1:0));
  if(incoming)check(incoming->RefCount==incomingCount+(changed?1:0));
  if(old && !freed[0])check(old->RefCount==counts[oldCase]-(changed?1:0));
  if(released)check(callbackMode==1?freed[2]:callbackMode==3?!freed[0]:freed[0]);
  cleanup=true;for(i=0;i<3;++i)if(!freed[i])delete blocks[i];
  ++cases;
 }
 if(failures){printf("FAIL %u\n",failures);return 1;}
 printf("FONT_POINTER_SHARING_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;}
