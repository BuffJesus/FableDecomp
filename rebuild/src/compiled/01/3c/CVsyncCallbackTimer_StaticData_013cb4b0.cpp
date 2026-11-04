#include "fable_display_viewport.h"

// FableWin.pdb names; retail accesses in GetInfo and the timer constructor.
// All five objects occupy the zero-filled tail of retail .data.
volatile CVsyncCallbackTimer::CGPUInfoBlock CVsyncCallbackTimer::GPUBlocks[2];
double CVsyncCallbackTimer::CPUTimeStamp;
double CVsyncCallbackTimer::CPUPrevTimeStamp;
long CVsyncCallbackTimer::CPUFrameCount;
volatile long CVsyncCallbackTimer::LatestBlockIndex;
