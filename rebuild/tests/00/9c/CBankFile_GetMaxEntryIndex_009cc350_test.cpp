
#include <stdio.h>
#include <string.h>
#include "fable_game_text_bank.h"
static CBankFile* bank;
static unsigned calls,failed,mode,validIndex;
static unsigned seen[32];
static CBankFile::CRuntimeData replacement[17];
static void check(bool v){if(!v)++failed;}
unsigned long CPackedUIntArray::operator[](unsigned long index) const {
 check(this==&bank->PackedValid);check(calls<32);seen[calls++]=index;
 if(mode==1)bank->Size=0;
 if(mode==2)bank->PackedValid.Size=0;
 if(mode==3){bank->PackedValid.Size=0;bank->RuntimeData._M_start=replacement;}
 return index+1==validIndex?0xffffffffUL:0;
}
int main(){unsigned cases=0;const unsigned bytes[]={1,0x7f,0xff};
 for(unsigned size=0;size<17;++size)for(validIndex=0;validIndex<17;++validIndex)for(unsigned packed=0;packed<2;++packed)for(unsigned b=0;b<3;++b){
  unsigned long words[0x110/4];memset(words,0xa5,sizeof(words));bank=reinterpret_cast<CBankFile*>(words);CBankFile::CRuntimeData data[17];memset(data,0,sizeof(data));reinterpret_cast<unsigned char*>(&data[validIndex])[9]=static_cast<unsigned char>(bytes[b]);bank->Size=size;bank->RuntimeData._M_start=data;bank->PackedValid.Size=packed;calls=mode=0;
  unsigned long before[0x110/4];memcpy(before,words,sizeof(words));long result=bank->GetMaxEntryIndex();long expected=validIndex>0 && validIndex<size?static_cast<long>(validIndex):-1;
  check(result==expected && memcmp(before,words,sizeof(words))==0);
  unsigned expectedCalls=packed && size>1?(expected>=0?size-1-validIndex+1:size-1):0;check(calls==expectedCalls);for(unsigned i=0;i<calls;++i)check(seen[i]==size-2-i);++cases;
 }
 for(mode=1;mode<4;++mode){unsigned long words[0x110/4];memset(words,0,sizeof(words));bank=reinterpret_cast<CBankFile*>(words);CBankFile::CRuntimeData data[17];memset(data,0,sizeof(data));memset(replacement,0,sizeof(replacement));data[2].Valid=true;replacement[3].Valid=true;bank->Size=6;bank->RuntimeData._M_start=data;bank->PackedValid.Size=6;calls=0;validIndex=0;
  check(bank->GetMaxEntryIndex()==(mode==1?-1:mode==2?2:3));check(calls==1 && seen[0]==4);++cases;
 }
 const unsigned high[]={0x80000001,0x80000002,0xffffffff};
 for(unsigned h=0;h<3;++h){unsigned long words[0x110/4];memset(words,0,sizeof(words));bank=reinterpret_cast<CBankFile*>(words);bank->Size=high[h];calls=mode=0;check(bank->GetMaxEntryIndex()==static_cast<long>(high[h]-1) && calls==0);++cases;}
 unsigned long words[0x110/4];memset(words,0,sizeof(words));bank=reinterpret_cast<CBankFile*>(words);bank->Size=0x80000000;bank->PackedValid.Size=1;calls=mode=0;validIndex=0x7fffffff;check(bank->GetMaxEntryIndex()==0x7fffffff && calls==1 && seen[0]==0x7ffffffe);++cases;
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("BANK_MAX_ENTRY_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
