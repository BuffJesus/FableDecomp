#include "rebuild_abi.h"
#include <cstdio>
struct Pred { void And(Pred* p); };
static int hit=0;
void Pred::And(Pred* p){ hit=1; }
extern "C" Pred* FABLE_FASTCALL Op_c0def0(Pred* self, Pred* rhs){ self->And(rhs); return self; }
int main(){ Pred a,b; if(Op_c0def0(&a,&b)!=&a){std::printf("B1");return 1;} if(!hit){std::printf("B2");return 1;} std::printf("RB_c0def0_OK\n"); return 0; }