#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4];
int main(){CDisplayManager* object=reinterpret_cast<CDisplayManager*>(storage);unsigned long state=0x937ac151;unsigned char expected[sizeof(storage)];
 for(unsigned i=0;i<4096;++i){
  memset(storage,static_cast<unsigned char>(i),sizeof(storage));state=state*1664525+1013904223;long x=static_cast<long>(state);state=state*1664525+1013904223;long y=static_cast<long>(state);object->RenderTargetDimensionsStorage_[0]=x;object->RenderTargetDimensionsStorage_[1]=y;memcpy(expected,storage,sizeof(storage));
  const C2DExtentsI dimensions=object->GetRenderTargetDimensions();if(dimensions.X!=x || dimensions.Y!=y || memcmp(expected,storage,sizeof(storage)))return 1;
 }
 printf("FABLETLC_RENDER_TARGET_DIMENSIONS_BEHAVIOR PASS scenarios=4096\n");return 0;
}
