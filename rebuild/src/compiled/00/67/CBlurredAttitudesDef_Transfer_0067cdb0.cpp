#include "rebuild_abi.h"
// CBlurredAttitudesDef::Transfer @ 0x0067cdb0
// add ecx,0x3c; push ecx; ecx=arg; call arg->Method(this+0x3c); ret 4
struct Target { void Xfer(void *sub); };
struct CBlurredAttitudesDef { char pad[0x3c]; char sub[4]; void Transfer(Target *t); };
void CBlurredAttitudesDef::Transfer(Target *t){
    t->Xfer((void*)&sub[0]);
}