#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
static const CWideString* expectedName;static CWideString* expectedCopy;static int calls;
CWideString::CWideString(const CWideString& other){if(&other!=expectedName || this!=expectedCopy)abort();++calls;memcpy(this,&other,sizeof(*this));}
int main(){
 for(int i=0;i<2;++i){
  double srcRaw[(sizeof(CProfile)+7)/8],dstRaw[(sizeof(CProfile)+16+7)/8];memset(srcRaw,0xa5,sizeof(srcRaw));memset(dstRaw,0xcd,sizeof(dstRaw));
  CProfile* src=reinterpret_cast<CProfile*>(srcRaw);CProfile* dst=reinterpret_cast<CProfile*>(reinterpret_cast<char*>(dstRaw)+8);
  src->Manager=i?reinterpret_cast<CProfileManager*>(0x123400):0;src->StartTime=0.125;src->TotalTime=-25.5;src->LastTime=150;src->Running=i!=0;src->Percentage=0.8;src->TreeDepth=-2;src->NewNode=i==0;src->Visible=17;src->CallCounter=0x7766;src->UpdatedCallCounter=-19;
  expectedName=&src->Name;expectedCopy=&dst->Name;calls=0;new(dst) CProfile(*src);
  if(calls!=1)return 1;
  const int fields[][2]={{0,4},{8,25},{0x28,17},{0x3c,12}};
  unsigned char expected[sizeof(CProfile)];memset(expected,0xcd,sizeof(expected));
  for(int j=0;j<4;++j)memcpy(expected+fields[j][0],reinterpret_cast<char*>(src)+fields[j][0],fields[j][1]);
  if(memcmp(expected,dst,sizeof(expected)))return 2;
  for(int j=0;j<8;++j)if(reinterpret_cast<unsigned char*>(dstRaw)[j]!=0xcd || reinterpret_cast<unsigned char*>(dst)[sizeof(*dst)+j]!=0xcd)return 3;
 }
 puts("PROFILE_COPY_PASS");return 0;
}