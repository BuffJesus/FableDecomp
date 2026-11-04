#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
int main(){double raw[8];memset(raw,0xa5,sizeof(raw));CResourceList* p=reinterpret_cast<CResourceList*>(raw);p->CurrentFrame=0xffffffffUL;p->UnloadedThisFrame=17;
 p->FrameEnd();if(p->CurrentFrame || p->UnloadedThisFrame || (unsigned long)p->AllocatedMemory!=0xa5a5a5a5UL)return 1;
 p->FrameEnd();if(p->CurrentFrame!=1)return 2;puts("FRAME_END_PASS");return 0;}