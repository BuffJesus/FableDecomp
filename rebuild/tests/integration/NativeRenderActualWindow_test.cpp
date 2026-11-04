#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"
static unsigned char arena[0x5000];
static CDisplayManager* expectedManager;
static const void* expectedWindow;
static unsigned calls,errors;
static void observe(CDisplayManager* receiver,const void* window)
{
 if(receiver!=expectedManager || window!=expectedWindow)++errors;
 ++calls;
 // The boundary can replace the display manager while a call is in flight.
 ((CRenderManagerCore*)arena)->DisplayManager=(CDisplayManager*)(arena+0x4800);
}
void CDisplayManager::SetViewport(const C2DBoxF& window){observe(this,&window);}
void CDisplayManager::SetIntegerViewportEndpoint(const C2DBoxI& window){observe(this,&window);}
int main()
{
 unsigned scenarios=0;
 for(unsigned type=0;type<2;++type)
  for(unsigned manager=0;manager<4;++manager)
   for(unsigned input=0;input<64;++input)
   {
    memset(arena,(int)(input*13+type),sizeof(arena));CRenderManagerCore* core=(CRenderManagerCore*)arena;
    expectedManager=(CDisplayManager*)(arena+0x4000+manager*0x100);core->DisplayManager=expectedManager;
    expectedWindow=arena+0x4c00+input*4;unsigned before=calls;
    if(type)core->SetAWindow(*(const C2DBoxI*)expectedWindow);else core->SetAWindow(*(const C2DBoxF*)expectedWindow);
    if(calls!=before+1 || core->DisplayManager!=(CDisplayManager*)(arena+0x4800))++errors;
    ++scenarios;
   }
 if(errors){printf("NATIVE_RENDER_ACTUAL_WINDOW_FAIL errors=%u\n",errors);return 1;}
 printf("NATIVE_RENDER_ACTUAL_WINDOW_PASS scenarios=%u\n",scenarios);return 0;
}
