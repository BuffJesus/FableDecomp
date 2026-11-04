#include "fable_thing_components.h"
#include <stdio.h>
#include <string.h>
int main(){unsigned scenarios=0;for(unsigned pattern=0;pattern<4;++pattern)for(unsigned starting=0;starting<256;++starting)for(unsigned blocking=0;blocking<256;++blocking){unsigned long storage[80],before[80];memset(storage,pattern*71,sizeof(storage));CTCCombat* combat=reinterpret_cast<CTCCombat*>(storage+2);((unsigned char*)combat)[0x62]=(unsigned char)starting;((unsigned char*)combat)[0x63]=(unsigned char)blocking;memcpy(before,storage,sizeof(storage));bool result=combat->IsBlocking();unsigned char raw;memcpy(&raw,&result,1);if(raw!=blocking || memcmp(before,storage,sizeof(storage)))return 1;++scenarios;}printf("IS_BLOCKING_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;}
