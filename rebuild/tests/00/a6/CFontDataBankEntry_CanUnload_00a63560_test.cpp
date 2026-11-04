#include <stdio.h>
#include <string.h>
#include "engine/CFontDataBankEntry.h"
int main(){
 double raw[(sizeof(CFontDataBankEntry)+7)/8];memset(raw,0,sizeof(raw));
 CFontDataBankEntry* e=reinterpret_cast<CFontDataBankEntry*>(raw);
 CCPPointerInfo info(0,0);
 const unsigned long counts[]={0,1,2,0xFFFFFFFFul};
 for(int data=0;data<2;++data)for(int block=0;block<2;++block)for(int k=0;k<4;++k){
  e->Font.Data=data?(CFontBank*)0x123400:0;e->Font.Info=block?&info:0;info.RefCount=counts[k];
  unsigned char before[sizeof(raw)];memcpy(before,raw,sizeof(raw));
  bool expected=data&&block&&counts[k]==1;
  if(e->CFontDataBankEntry::CanUnload()!=expected||memcmp(before,raw,sizeof(raw))||info.RefCount!=counts[k])return 1;
 }
 puts("FONT_00a63560_PASS");return 0;
}
