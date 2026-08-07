#include "rebuild_abi.h"
#include <cstdio>
struct FableEffectResult_009081d0 { const char* seen; void* ctor(const char* text); };
void* FableEffectResult_009081d0::ctor(const char* text) { seen=text; return this; }
extern "C" char FABLE_STDCALL FableGetEffectName_009081d0(FableEffectResult_009081d0* self);
int main() {
    FableEffectResult_009081d0 r; r.seen=0;
    char got=FableGetEffectName_009081d0(&r);
    if(got!=1) { std::printf("BAD_RET\n"); return 1; }
    if(r.seen==0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETEFFECT_009081d0_OK\n"); return 0;
}