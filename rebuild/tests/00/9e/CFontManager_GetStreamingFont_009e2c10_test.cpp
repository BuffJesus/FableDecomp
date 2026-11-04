#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fable_system_manager.h"
#include "engine/CStreamingFontDataBank.h"
#include "engine/CStreamingFontDataBankEntry.h"
#include "test_support/base_class_stubs.h"
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CFontManager::CFontManager(const CCharString&,const CCharString&) {}
CFontManager::~CFontManager() {}
template<class T>CCountedPointer<T>::CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
struct CFontManagerTestAccess {static void set(CFontManager& m,CStreamingFontDataBank* bank){m.PStreamingFontDataBank.Data=bank;}};
static unsigned long nameStorage;
static CStreamingFontDataBank* expectedBank=(CStreamingFontDataBank*)0x111400;
static CStreamingFontDataBankEntry* expectedEntry=(CStreamingFontDataBankEntry*)0x222800;
static unsigned long expectedIndex;
static CFontBank* font;
static CCPPointerInfo* info;
static int phase,deleted;
const CStreamingFontDataBankEntry* CStreamingFontDataBank::PeekFontInfo(unsigned long index) const{
 if(this!=expectedBank||index!=expectedIndex||phase!=0)abort();phase=1;return expectedEntry;
}
CCountedPointer<CFontBank> CStreamingFontDataBankEntry::AcquireFont() const{
 if(this!=expectedEntry||phase!=1)abort();phase=2;
 CCountedPointer<CFontBank> result;result.Data=font;result.Info=info;if(info)++info->RefCount;return result;
}
static void FABLE_FASTCALL Destroy(void* p){if(p!=font)abort();++deleted;}
int main(){
 const CCharString& name=*reinterpret_cast<CCharString*>(&nameStorage);
 CFontManager manager(name,name);CFontManagerTestAccess::set(manager,expectedBank);
 double fontStorage[(sizeof(CFontBank)+16+7)/8];
 font=reinterpret_cast<CFontBank*>(reinterpret_cast<char*>(fontStorage)+8);
 for(int owned=0;owned<2;++owned){
  CCountedPointer<CFontBank> owner;owner.Data=font;
  owner.Info=info=owned?new CCPPointerInfo(&Destroy,font):0;
  for(int i=0;i<3;++i){
   memset(fontStorage,0xA5,sizeof(fontStorage));
   unsigned char expected[sizeof(fontStorage)];memcpy(expected,fontStorage,sizeof(fontStorage));
   reinterpret_cast<CFontBank*>(expected+8)->PManager=&manager;
   expectedIndex=i==2?0xFFFFFFFFul:(unsigned long)i;phase=0;
   {CCountedPointer<CFontBank> result=manager.GetStreamingFont(expectedIndex);
    if(phase!=2||result.Data!=font||result.Info!=info||memcmp(expected,fontStorage,sizeof(fontStorage)))return 1;
    if(info&&info->RefCount!=2)return 2;
   }
   if(info&&info->RefCount!=1)return 3;
   if(deleted)return 4;
  }
 }
 if(deleted!=1)return 5;
 puts("FONT_009e2c10_PASS");return 0;
}
