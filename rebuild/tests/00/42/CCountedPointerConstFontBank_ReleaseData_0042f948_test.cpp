#include <stdio.h>
#include <stdlib.h>
#include "engine/CFontBank.h"
#include "fable_counted_pointer.h"
static int deleted,blocksFreed;static CCPPointerInfo* watched;
void __cdecl operator delete(void* p){if(p==watched)++blocksFreed;free(p);}
static void FABLE_FASTCALL DeleteFont(void* p){if(p!=(void*)0x123400)abort();++deleted;}
template<class T> int Check(){
 for(int mode=0;mode<3;++mode){
  double raw[1];CCountedPointer<T>* p=reinterpret_cast<CCountedPointer<T>*>(raw);
  p->Data=(T*)0x123400;watched=0;deleted=blocksFreed=0;
  if(mode){watched=(CCPPointerInfo*)malloc(sizeof(CCPPointerInfo));watched->RefCount=mode;watched->DeleteFunc=DeleteFont;watched->Data=(void*)p->Data;}
  p->Info=watched;p->ReleaseData();
  if(p->Data||p->Info||deleted!=(mode==1)||blocksFreed!=(mode==1))return 1;
  p->ReleaseData();if(deleted!=(mode==1)||blocksFreed!=(mode==1))return 2;
  if(mode==2){if(watched->RefCount!=1)return 3;free(watched);}
 }
 return 0;
}

int main(){if(Check<const CFontBank>())return 1;puts("FONT_0042f948_PASS");return 0;}
