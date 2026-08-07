#include "rebuild_abi.h"
// operator&& @ 0x008cff40
struct Pred;
struct Sub { void And(Pred *self); };
struct GObj { char pad[4]; Sub *m_sub; };
extern GObj *g_pred_8cff40;
struct Pred { char d; Pred *Op(); };
Pred *Pred::Op(){
    g_pred_8cff40->m_sub->And(this);
    return this;
}