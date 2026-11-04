#include "fable_boot.h"
#include <stdio.h>
#include <stdlib.h>
#include <new>
static unsigned long allocated,released,destroyed;
void* FABLE_CDECL FableOperatorNew(fable_u32 size){if(size!=12)exit(8);++allocated;return ::operator new(size);}
void FABLE_CDECL FableOperatorDelete(void* p){++released;::operator delete(p);}
class TestObject{public:virtual ~TestObject(){++destroyed;}};
static void require(bool yes){if(!yes)exit(2);}
int main(){for(unsigned long i=0;i<32;++i){CCountedProgressDisplay empty;{CCountedProgressDisplay original((CProgressDisplay*)new TestObject);FableReferenceCount* block=original.reference;require(block&&block->owners==1&&block->object==original.object);{CCountedProgressDisplay copy(original);require(block->owners==2);SetProgressDisplay(&copy);require(block->owners==3&&g_FableProgressDisplay_013CAA38.reference==block);{CCountedProgressDisplay result=GetProgressDisplay();require(result.reference==block&&result.object==original.object&&block->owners==4);}require(block->owners==3);SetProgressDisplay(&empty);require(block->owners==2&&g_FableProgressDisplay_013CAA38.object==0&&g_FableProgressDisplay_013CAA38.reference==0);}require(block->owners==1&&destroyed==i&&released==i);}require(destroyed==i+1&&released==i+1&&allocated==i+1);}printf("PROGRESS_COUNTED_LIFETIME_OK scenarios=32 destroyed=%lu blocks_released=%lu\n",destroyed,released);return 0;}
