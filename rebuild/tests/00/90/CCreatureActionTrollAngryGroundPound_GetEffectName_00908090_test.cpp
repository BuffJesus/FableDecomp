#include "rebuild_abi.h"
#include <cstdio>
struct FableEffectResult_00908090 { const char* seen; void* ctor(const char* text); };
void* FableEffectResult_00908090::ctor(const char* text) { seen=text; return this; }
extern "C" char FABLE_STDCALL FableGetEffectName_00908090(FableEffectResult_00908090* self);
int main() {
    FableEffectResult_00908090 r; r.seen=0;
    char got=FableGetEffectName_00908090(&r);
    if(got!=1) { std::printf("BAD_RET\n"); return 1; }
    if(r.seen==0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETEFFECT_00908090_OK\n"); return 0;
}