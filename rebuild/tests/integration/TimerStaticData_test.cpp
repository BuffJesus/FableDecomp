#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>

static unsigned cleanupCalls;
void CVsyncCallbackTimer::ClearAllQueries() { ++cleanupCalls; }
namespace _STL {
template<> void list<CVsyncCallbackTimer::CPendingQuery>::clear() {}
}

struct CVsyncCallbackTimerTestAccess {
    static bool Initial() {
        if (CVsyncCallbackTimer::CPUTimeStamp || CVsyncCallbackTimer::CPUPrevTimeStamp ||
            CVsyncCallbackTimer::CPUFrameCount || CVsyncCallbackTimer::LatestBlockIndex) return false;
        for (unsigned i=0; i<2; ++i) {
            const volatile CVsyncCallbackTimer::CGPUInfoBlock& b=CVsyncCallbackTimer::GPUBlocks[i];
            if (b.GPUTimeStamp || b.GPUPrevTimeStamp || b.GPUFrameCount || b.Locked) return false;
        }
        return true;
    }
    static void Fill(unsigned n, unsigned latest) {
        CVsyncCallbackTimer::CPUTimeStamp=n+0.25;
        CVsyncCallbackTimer::CPUPrevTimeStamp=-double(n)-0.5;
        CVsyncCallbackTimer::CPUFrameCount=long(n)-128;
        CVsyncCallbackTimer::LatestBlockIndex=latest;
        for (unsigned i=0; i<2; ++i) {
            volatile CVsyncCallbackTimer::CGPUInfoBlock& b=CVsyncCallbackTimer::GPUBlocks[i];
            b.GPUTimeStamp=n+100.5+i*100;
            b.GPUPrevTimeStamp=-double(n)-200.25-i*100;
            b.GPUFrameCount=long(n)+300+i*100;
            b.Locked=long(n)+i;
        }
    }
    static bool Locks(unsigned n) {
        return CVsyncCallbackTimer::GPUBlocks[0].Locked==long(n) &&
               CVsyncCallbackTimer::GPUBlocks[1].Locked==long(n)+1;
    }
};

int main() {
    if (!CVsyncCallbackTimerTestAccess::Initial()) return 1;
    unsigned scenarios=0;
    for (unsigned n=0; n<256; ++n) for (unsigned latest=0; latest<2; ++latest) {
        CVsyncCallbackTimerTestAccess::Fill(n,latest);
        CVsyncCallbackTimer timer;
        if (!CVsyncCallbackTimerTestAccess::Initial()) return 2;
        CVsyncCallbackTimerTestAccess::Fill(n,latest);
        unsigned char before[sizeof(timer)]; memcpy(before,&timer,sizeof(timer));
        CVsyncCallbackTimerInfo info=timer.GetInfo();
        if (info.CPUTimeStamp!=n+0.25 || info.CPUPrevTimeStamp!=-double(n)-0.5 ||
            info.CPUFrameCount!=long(n)-128 || info.GPUTimeStamp!=n+100.5+latest*100 ||
            info.GPUPrevTimeStamp!=-double(n)-200.25-latest*100 ||
            info.GPUFrameCount!=long(n)+300+latest*100 ||
            !CVsyncCallbackTimerTestAccess::Locks(n) || memcmp(before,&timer,sizeof(timer))) return 3;
        ++scenarios;
    }
    if (cleanupCalls!=scenarios) return 4;
    printf("TIMER_STATIC_DATA_BEHAVIOR_PASS scenarios=%u\n",scenarios);
    return 0;
}
