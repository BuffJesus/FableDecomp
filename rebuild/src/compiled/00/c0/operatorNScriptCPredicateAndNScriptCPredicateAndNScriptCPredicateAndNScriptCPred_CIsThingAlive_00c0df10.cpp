#include "rebuild_abi.h"
struct Pred { void And(Pred* p); };
extern "C" Pred* FABLE_FASTCALL Op_c0df10(Pred* self, Pred* rhs){ self->And(rhs); return self; }