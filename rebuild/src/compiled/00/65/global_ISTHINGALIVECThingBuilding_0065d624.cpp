#include "rebuild_abi.h"
struct CThingBuilding { char pad[0x91]; unsigned char m_flags; };
struct Wrap { CThingBuilding *m_p; };
extern "C" char FABLE_FASTCALL FableIsAlive_65d624(Wrap *w){
    CThingBuilding *p=w->m_p; if(p && !(p->m_flags&1)) return 1; return 0; }