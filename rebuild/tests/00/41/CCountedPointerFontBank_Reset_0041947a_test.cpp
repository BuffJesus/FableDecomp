#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_bank.h"
#include "fable_counted_pointer.h"
static bool failBlock;static int blockAllocations,releaseCalls,fontDeleted;
static CFontBank* expectedOld;static CCPPointerInfo* expectedInfo;
void* __cdecl operator new(size_t n){if(n==sizeof(CCPPointerInfo)){++blockAllocations;if(failBlock)return 0;}return malloc(n);}
void __cdecl operator delete(void* p){free(p);}


class TestFont:public CFontBank{
public:virtual ~TestFont(){++fontDeleted;}
virtual C2DExtentsF GetStringExtents(const wchar_t*,bool)const{return C2DExtentsF(0,0);}
virtual float GetFontMaxHeight()const{return 0;} virtual void PrepareToRender(bool)const{}
virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{}
};
template<> void CCountedPointer<CFontBank>::ReleaseData(){
 if(Data!=expectedOld||Info!=expectedInfo)abort();++releaseCalls;Data=0;Info=0;
}
int main(){
 double raw[1];CCountedPointer<CFontBank>* p=reinterpret_cast<CCountedPointer<CFontBank>*>(raw);
 for(int mode=0;mode<3;++mode){
  TestFont* f=mode?new TestFont:0;
  p->Data=expectedOld=(CFontBank*)0x112200;p->Info=expectedInfo=(CCPPointerInfo*)0x225500;
  failBlock=mode==2;releaseCalls=blockAllocations=0;
  p->Reset(f);
  if(releaseCalls!=1||p->Data!=f||blockAllocations!=(mode?1:0))return 1;
  if(mode==1){
   if(!p->Info||p->Info->RefCount!=1||p->Info->Data!=f||!p->Info->DeleteFunc)return 2;
   // The reconstructed deleter must dispatch the virtual deleting destructor.
   p->Info->DeleteFunc(p->Info->Data);delete p->Info;p->Info=0;
   if(fontDeleted!=1)return 3;
  }else{
   if(p->Info)return 4;
   if(f)delete f;
  }
 }
 if(fontDeleted!=2 || g_CCharStringInstanceCount_013BD800!=0)return 5;
 puts("FONT_0041947a_PASS");return 0;
}

#include "../../../src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp"
#include "../../../src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp"
#include "../../../src/compiled/00/99/CCharString_Destructor_0099eae0.cpp"
#include "../../../src/compiled/00/99/CCharString_UnassignString_0099e9b0.cpp"
#include "../../../src/compiled/00/9a/CFontBank_Preload_009a0b20.cpp"
fable_i32 g_CCharStringInstanceCount_013BD800=0;
