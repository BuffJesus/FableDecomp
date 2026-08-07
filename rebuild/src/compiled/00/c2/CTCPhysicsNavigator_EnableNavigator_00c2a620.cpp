#include "rebuild_abi.h"
extern int g_vt_c2a620;
struct C { void* vt; char f4; C* Enable(char a); };
C* C::Enable(char a){ this->vt=&g_vt_c2a620; this->f4=a; return this; }