// CVsyncCallbackTimer::GetInfo (retail 0x009F7050; the body is unoptimised in
// retail: frame pointer, `this` spilled, out-of-line CVsyncCallbackTimerInfo ctor).
// FableWin.pdb ?GetInfo@CVsyncCallbackTimer@@QAE?AUCVsyncCallbackTimerInfo@@XZ.
// Pins the latest GPU block (Locked count) while copying it and the CPU stamps.
#pragma optimize("", off)
#pragma inline_depth(0)
#include "fable_display_viewport.h"

CVsyncCallbackTimerInfo CVsyncCallbackTimer::GetInfo()
{
    volatile CGPUInfoBlock* block = &GPUBlocks[LatestBlockIndex];
    InterlockedIncrement(&block->Locked);
    CVsyncCallbackTimerInfo info;
    info.CPUFrameCount = CPUFrameCount;
    info.GPUFrameCount = block->GPUFrameCount;
    info.CPUTimeStamp = CPUTimeStamp;
    info.CPUPrevTimeStamp = CPUPrevTimeStamp;
    info.GPUTimeStamp = block->GPUTimeStamp;
    info.GPUPrevTimeStamp = block->GPUPrevTimeStamp;
    InterlockedDecrement(&block->Locked);
    return info;
}