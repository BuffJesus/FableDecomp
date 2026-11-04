#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CStreamingFontDataBankEntry.h"
#include "engine/CStreamingFontBank.h"
fable_i32 g_CCharStringInstanceCount_013BD800=0;
static CStreamingFontDataBankEntry* entry;
static CStreamingFontDataBank* originalBank=(CStreamingFontDataBank*)0x112200;
static unsigned long originalIndex;
static bool failAllocation;
static int allocations,constructors,resets,deleted;
static void* allocated;
void* __cdecl operator new(size_t n){
 if(n!=sizeof(CStreamingFontBank))abort();++allocations;
 // Retail snapshots constructor inputs before calling the allocator.
 entry->Bank=(CStreamingFontDataBank*)0x887700;entry->Index=0xDEADBEEF;
 return allocated=failAllocation?0:malloc(n);
}
void __cdecl operator delete(void* p){free(p);}
CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* bank,unsigned long index){
 if(bank!=originalBank||index!=originalIndex||this!=allocated)abort();++constructors;
}
static void FABLE_FASTCALL DeleteFont(void* p){if(p!=allocated)abort();++deleted;static_cast<CStreamingFontBank*>(p)->~CStreamingFontBank();free(p);}
template<> void CCountedPointer<CFontBank>::Reset(CFontBank* p){
 if(this!=&entry->Font||p!=allocated||Data||Info)abort();++resets;Data=p;
 if(p){Info=(CCPPointerInfo*)malloc(sizeof(CCPPointerInfo));Info->RefCount=1;Info->DeleteFunc=&DeleteFont;Info->Data=p;}
}
int main(){
 double raw[(sizeof(CStreamingFontDataBankEntry)+7)/8];entry=reinterpret_cast<CStreamingFontDataBankEntry*>(raw);
 for(int fail=0;fail<2;++fail){
  memset(raw,0,sizeof(raw));entry->Bank=originalBank;entry->Index=originalIndex=0xF1234567ul;
  failAllocation=fail!=0;allocations=constructors=resets=deleted=0;
  {CCountedPointer<CFontBank> result=entry->AcquireFont();
   if(allocations!=1||constructors!=(fail?0:1)||resets!=1||result.Data!=allocated||result.Info!=entry->Font.Info)return 1;
   if(fail){if(result.Data||result.Info)return 2;}
   else if(!result.Info||result.Info->RefCount!=2)return 3;
  }
  if(!fail){
   if(entry->Font.Info->RefCount!=1)return 4;
   {CCountedPointer<CFontBank> cached=entry->AcquireFont();
    if(allocations!=1||constructors!=1||resets!=1||cached.Data!=allocated||cached.Info->RefCount!=2)return 5;
   }
   if(entry->Font.Info->RefCount!=1)return 6;
  }
  entry->Font.~CCountedPointer<CFontBank>();
  if(deleted!=(fail?0:1))return 7;
 }
 // A cached raw font with no ownership block is returned without allocating.
 memset(raw,0,sizeof(raw));entry->Font.Data=(CFontBank*)0x123400;allocations=constructors=resets=0;
 {CCountedPointer<CFontBank> result=entry->AcquireFont();
  if(result.Data!=entry->Font.Data||result.Info||allocations||constructors||resets)return 8;
 }
 if(g_CCharStringInstanceCount_013BD800!=0)return 9;
 puts("FONT_00a5f1c0_PASS");return 0;
}

CStreamingFontBank::~CStreamingFontBank(){}
C2DExtentsF CStreamingFontBank::GetStringExtents(const wchar_t*,bool)const{abort();return C2DExtentsF();}
void CStreamingFontBank::PrepareToRender(bool)const{abort();}
void CStreamingFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{abort();}
bool CStreamingFontBank::Preload(const CWideString&,bool)const{abort();return false;}

#include "../../../src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp"
#include "../../../src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp"
#include "../../../src/compiled/00/99/CCharString_Destructor_0099eae0.cpp"
#include "../../../src/compiled/00/99/CCharString_UnassignString_0099e9b0.cpp"
#include "../../../src/compiled/00/9a/CFontBank_Preload_009a0b20.cpp"
#include "../../../src/compiled/00/9a/CStreamingFontBank_GetFontMaxHeight_009a0ea0.cpp"
