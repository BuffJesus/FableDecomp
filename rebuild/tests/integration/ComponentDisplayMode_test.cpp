#include "fable_game.h"
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_display_engine.h"
#include "fable_surface.h"
#include <stdio.h>
#include <string.h>
CDisplayManager* g_FableDisplayManager_013B8390;
static unsigned char comp[0x200],world[0x200],engine[0x100],display[3][0x300];
static unsigned seq,errors,rebind;static unsigned long w,h,d,status;
static CDisplayManager* Manager(unsigned i) {return reinterpret_cast<CDisplayManager*>(display[i]);}
static const void* CachedSurface() {return display[(rebind&1)?1:0]+0x2c;}
static const void* LatestSurface() {return display[(rebind&2)?2:((rebind&1)?1:0)]+0x2c;}
void CDisplayEngine::PreChangeResolution() {if(++seq!=1 || this!=(void*)engine) ++errors;}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetDisplayMode(const C2DExtentsI& dimensions,unsigned long depth,ESurfaceMultisampleType sample,unsigned long refresh) {
    if(++seq!=2 || this!=Manager(0) || (unsigned long)dimensions.X!=w || (unsigned long)dimensions.Y!=h || depth!=d || sample!=SURFACE_MULTISAMPLE_NONE || refresh) ++errors;
    g_FableDisplayManager_013B8390=Manager((rebind&1)?1:0);return static_cast<ECreateDeviceSuccess>(status);
}
C2DExtentsI CSurface::GetDimensions() const {if(++seq!=3 || this!=CachedSurface()) ++errors;g_FableDisplayManager_013B8390=Manager((rebind&2)?2:((rebind&1)?1:0));return C2DExtentsI((long)0x81234567UL,4444);}
CPixelFormat CSurface::GetPixelFormat() const {if(++seq!=4 || this!=LatestSurface()) ++errors;CPixelFormat f;f.FormatIndex=31;return f;}
unsigned long CPixelFormat::GetColourDepth() const {if(++seq!=5 || FormatIndex!=31) ++errors;return 32;}
unsigned long CSurface::GetHeight() const {if(++seq!=6 || this!=CachedSurface()) ++errors;return 0x92345678UL;}
void CMainGameComponent::PostChangeResolution(unsigned long width,unsigned long height,unsigned long depth) {if(++seq!=7 || this!=(void*)comp || width!=0x81234567UL || height!=0x92345678UL || depth!=32) ++errors;}
static int Run(unsigned long width,unsigned long height,unsigned char depth,unsigned& scenarios) {
    for(unsigned result=0;result<2;++result) for(rebind=0;rebind<4;++rebind) for(unsigned composed=0;composed<2;++composed) {
        memset(comp,0xa9,sizeof(comp));memset(world,0xa9,sizeof(world));memset(engine,0xa9,sizeof(engine));memset(display,0xa9,sizeof(display));void* e=engine;void* c=comp;memcpy(comp+0x28,&e,4);memcpy(world+8,&c,4);
        unsigned char compBefore[sizeof(comp)],worldBefore[sizeof(world)],engineBefore[sizeof(engine)],displayBefore[sizeof(display)];memcpy(compBefore,comp,sizeof(comp));memcpy(worldBefore,world,sizeof(world));memcpy(engineBefore,engine,sizeof(engine));memcpy(displayBefore,display,sizeof(display));
        CGameEvent event(0x7fffffff);unsigned short ww=(unsigned short)width,hh=(unsigned short)height;memcpy(event.Data,&ww,2);memcpy(event.Data+2,&hh,2);event.Data[4]=depth;event.EndPos=5;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
        seq=errors=0;status=result?0xffffffffUL:0;w=composed?ww:width;h=composed?hh:height;d=depth;g_FableDisplayManager_013B8390=Manager(0);
        if(composed) reinterpret_cast<CWorld*>(world)->LEASetDisplayMode(&event);else reinterpret_cast<CMainGameComponent*>(comp)->SetDisplayMode(width,height,depth);++scenarios;
        if(errors || seq!=7 || g_FableDisplayManager_013B8390!=Manager((rebind&2)?2:((rebind&1)?1:0)) || memcmp(compBefore,comp,sizeof(comp)) || memcmp(worldBefore,world,sizeof(world)) || memcmp(engineBefore,engine,sizeof(engine)) || memcmp(displayBefore,display,sizeof(display)) || memcmp(&eventBefore,&event,sizeof(event))) {printf("COMPONENT_DISPLAY_MODE_FAIL case=%u seq=%u errors=%u\n",scenarios,seq,errors);return 1;}
    }
    return 0;
}
int main() {
    unsigned scenarios=0;static const unsigned short dims[8]={0,1,255,256,0x8000,0xffff,1280,1920};static const unsigned char depths[6]={0,1,16,24,32,255};
    for(unsigned x=0;x<8;++x) for(unsigned y=0;y<8;++y) for(unsigned d=0;d<6;++d) if(Run(dims[x],dims[y],depths[d],scenarios)) return 1;
    for(unsigned d=0;d<256;++d) if(Run(1280,720,(unsigned char)d,scenarios)) return 2;
    if(Run(0xffffffffUL,0x80000000UL,0,scenarios) || Run(0x80000000UL,0xffffffffUL,255,scenarios) || Run(0x12345678UL,0x87654321UL,32,scenarios) || Run(0x10000UL,0x10001UL,16,scenarios)) return 3;
    printf("COMPONENT_DISPLAY_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
