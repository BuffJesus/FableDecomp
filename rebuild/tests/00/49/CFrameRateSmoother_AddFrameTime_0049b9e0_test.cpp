#include "fable_game.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int calls;
static CFrameRateSmoother* receiver;
static long expectedFirst, expectedLast, expectedCount;
static int badState;
void CFrameRateSmoother::CalculateSmoothedTime() {
    ++calls; receiver=this;
    if (FirstTime!=expectedFirst || LastTime!=expectedLast || NoTimes!=expectedCount) badState=1;
    SmoothedTime=123.5;
}
struct FrameLayout {
    unsigned char base[8];
    struct { float* _M_start; float* _M_finish; struct { float* _M_data; } _M_end_of_storage; } Times;
    long MaxNoTimes, NoTimes, NoFramesToChangeFrameRateOver, FirstTime, LastTime;
    double SmoothedTime;
};
FABLE_STATIC_ASSERT(sizeof(FrameLayout)==sizeof(CFrameRateSmoother));
int main() {
    const long capacity[]={3,3,3,1};
    const long count[]={0,2,3,1};
    const long first[]={0,1,1,0};
    const long last[]={0,0,1,0};
    const double value[]={0.5,8.0,-4.0,0.25};
    for (int c=0;c<4;++c) {
        union { double align; unsigned char bytes[0x30]; } storage, expected;
        float samples[]={99.0f,1.0f,2.0f,3.0f,101.0f};
        float expectedSamples[5]; memcpy(expectedSamples,samples,sizeof(samples));
        memset(storage.bytes,0xA5,sizeof(storage));
        FrameLayout* s=reinterpret_cast<FrameLayout*>(storage.bytes);
        s->Times._M_start=samples+1; s->Times._M_finish=samples+1+capacity[c];
        s->Times._M_end_of_storage._M_data=samples+1+capacity[c];
        s->MaxNoTimes=capacity[c];s->NoTimes=count[c];s->FirstTime=first[c];s->LastTime=last[c];
        memcpy(expected.bytes,storage.bytes,sizeof(storage));
        FrameLayout* e=reinterpret_cast<FrameLayout*>(expected.bytes);
        expectedFirst=(count[c]==capacity[c]) ? (first[c]+1)%capacity[c] : first[c];
        expectedLast=(last[c]+1)%capacity[c];expectedCount=(count[c]==capacity[c]) ? capacity[c] : count[c]+1;
        e->FirstTime=expectedFirst;e->LastTime=expectedLast;e->NoTimes=expectedCount;e->SmoothedTime=123.5;
        expectedSamples[1+last[c]]=static_cast<float>(value[c]);
        calls=badState=0;receiver=0;reinterpret_cast<CFrameRateSmoother*>(s)->AddFrameTime(value[c]);
        if (calls!=1 || receiver!=reinterpret_cast<CFrameRateSmoother*>(s) || badState) return 1;
        if (memcmp(storage.bytes,expected.bytes,sizeof(storage)) || memcmp(samples,expectedSamples,sizeof(samples))) return 2;
    }
    puts("FABLETLC_FRAME_SMOOTHER_ADD PASS");return 0;
}