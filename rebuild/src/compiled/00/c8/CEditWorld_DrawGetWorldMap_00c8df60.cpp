#include "rebuild_abi.h"
struct C { char p[0x1c]; int* f1c; };
extern "C" int FABLE_FASTCALL Get_c8df60(C* self, int i){ if(self->f1c) return self->f1c[i]; return -1; }