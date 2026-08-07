#include "rebuild_abi.h"
// IS_THING_ALIVE(CThingBuilding*) @ 0x0065d55f
struct CThingBuilding { char pad[0x91]; unsigned char m_flags; };
struct Wrap { CThingBuilding *m_p; };
extern "C" char FABLE_FASTCALL FableIsAlive_65d55f(Wrap *w){
    CThingBuilding *p = w->m_p;
    if (p && !(p->m_flags & 1)) return 1;
    return 0;
}