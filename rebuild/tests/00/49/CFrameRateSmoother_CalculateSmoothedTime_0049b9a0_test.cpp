#include "fable_game.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
struct FrameLayout {
    unsigned char base[8];
    struct { float* _M_start; float* _M_finish; struct { float* _M_data; } _M_end_of_storage; } Times;
    long MaxNoTimes, NoTimes, NoFramesToChangeFrameRateOver, FirstTime, LastTime;
    double SmoothedTime;
};
FABLE_STATIC_ASSERT(sizeof(FrameLayout)==sizeof(CFrameRateSmoother));
// Member shim provides access to the real private method without changing its ABI.
void CFrameRateSmoother::AddFrameTime(double) { CalculateSmoothedTime(); }
int main() {
    const long first[]={0,3,2,1};const long count[]={1,2,4,0};
    const double average[]={2.0,9.0,7.5,0.0};
    for (int c=0;c<4;++c) {
        union { double align; unsigned char bytes[0x30]; } storage, expected;
        float samples[]={2.0f,4.0f,8.0f,16.0f};float untouched[4];memcpy(untouched,samples,sizeof(samples));
        memset(storage.bytes,0xA5,sizeof(storage));
        FrameLayout* s=reinterpret_cast<FrameLayout*>(storage.bytes);
        s->Times._M_start=samples;s->Times._M_finish=samples+4;s->Times._M_end_of_storage._M_data=samples+4;
        s->MaxNoTimes=4;s->NoTimes=count[c];s->FirstTime=first[c];s->SmoothedTime=-1.0;
        memcpy(expected.bytes,storage.bytes,sizeof(storage));reinterpret_cast<CFrameRateSmoother*>(s)->AddFrameTime(0);
        if (count[c] ? s->SmoothedTime!=average[c] : !_isnan(s->SmoothedTime)) return 1;
        memcpy(expected.bytes+0x28,storage.bytes+0x28,8);
        if (memcmp(storage.bytes,expected.bytes,sizeof(storage)) || memcmp(samples,untouched,sizeof(samples))) return 2;
    }
    puts("FABLETLC_FRAME_SMOOTHER_CALCULATE PASS");return 0;
}