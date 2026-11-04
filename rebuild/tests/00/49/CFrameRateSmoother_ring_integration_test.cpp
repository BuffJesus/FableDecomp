#include "fable_game.h"
#include <stdio.h>
#include <string.h>
struct FrameLayout {
    unsigned char base[8];
    float* begin; float* end; float* capacityEnd;
    long capacity, count, transitionFrames, first, last;
    double average;
};
FABLE_STATIC_ASSERT(sizeof(FrameLayout)==sizeof(CFrameRateSmoother));
int main() {
    FrameLayout state; memset(&state,0,sizeof(state));
    float guarded[]={123.0f,0.0f,0.0f,0.0f,456.0f};
    state.begin=guarded+1;state.end=guarded+4;state.capacityEnd=guarded+4;state.capacity=3;
    CFrameRateSmoother* smoother=reinterpret_cast<CFrameRateSmoother*>(&state);
    const double expected[]={1.0,1.5,2.0,3.0,4.0,5.0,6.0};
    for (int i=0;i<7;++i) {
        smoother->AddFrameTime(i+1.0);
        if (smoother->GetSmoothedTime()!=expected[i]) return 1;
        if (state.count!=(i<3 ? i+1 : 3)) return 2;
        if (guarded[0]!=123.0f || guarded[4]!=456.0f) return 3;
    }
    if (state.first!=1 || state.last!=1 || guarded[1]!=7.0f || guarded[2]!=5.0f || guarded[3]!=6.0f) return 4;
    puts("FABLETLC_FRAME_SMOOTHER_RING PASS");return 0;
}
