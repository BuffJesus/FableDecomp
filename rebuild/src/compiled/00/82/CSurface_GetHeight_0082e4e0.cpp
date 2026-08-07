#include "rebuild_abi.h"
// CSurface::GetHeight @ 0x0082e4e0
struct FableSurfHelper_0082e4e0
{
    long compute(void* pargs);
};
struct FableSurf_0082e4e0
{
    char pad[0x20];
    FableSurfHelper_0082e4e0 sub;   // +0x20
    long GetHeight(long a, long b);
};
long FableSurf_0082e4e0::GetHeight(long a, long b)
{
    return this->sub.compute(&a) + 0x18;
}