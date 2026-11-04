#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineVideoOptionsDef.h"

// Inlined unsigned high-bit search visible in both donor and retail bodies.
static __forceinline long HighestSetBit(unsigned long value)
{
    if(!value)return -1;
    if(value>=0x80000000UL)return 31;
    unsigned long next=value+1;
    long bit=(static_cast<long>(0x10000UL-next)>>31)&16;
    bit+=(static_cast<long>((1UL<<(bit+8))-next)>>31)&8;
    bit+=(static_cast<long>((1UL<<(bit+4))-next)>>31)&4;
    bit+=(static_cast<long>((1UL<<(bit+2))-next)>>31)&2;
    bit+=(static_cast<long>((1UL<<(bit+1))-next)>>31)&1;
    return bit;
}

void CEngineLocalDetailGenerator::UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& quality)
{
    for(long i=0;i<8;++i) {
        RuntimeFadeDistanceModiferTable[i].DistanceClampMin=0.0f;
        RuntimeFadeDistanceModiferTable[i].DistanceFactor=1.0f;
    }
    RuntimeFadeDistanceModiferTable[1].DistanceClampMin=quality.DrawDistanceMinimum;
    RuntimeFadeDistanceModiferTable[1].DistanceFactor=quality.DrawDistanceMultiplier;
    RuntimeFadeDistanceModiferTable[2].DistanceClampMin=0.0f;
    RuntimeFadeDistanceModiferTable[2].DistanceFactor=quality.RepeatedMeshDrawDistanceFactor;
    RuntimeFadeDistanceModiferTable[4].DistanceClampMin=0.0f;
    RuntimeFadeDistanceModiferTable[4].DistanceFactor=1.0f;
    for(long group=0;group<8;++group) {
        unsigned long mask=group;
        while(mask) {
            unsigned long bit=1UL<<HighestSetBit(mask);
            if(RuntimeFadeDistanceModiferTable[group].DistanceClampMin<RuntimeFadeDistanceModiferTable[bit].DistanceClampMin)
                RuntimeFadeDistanceModiferTable[group].DistanceClampMin=RuntimeFadeDistanceModiferTable[bit].DistanceClampMin;
            if(RuntimeFadeDistanceModiferTable[group].DistanceFactor<RuntimeFadeDistanceModiferTable[bit].DistanceFactor)
                RuntimeFadeDistanceModiferTable[group].DistanceFactor=RuntimeFadeDistanceModiferTable[bit].DistanceFactor;
            mask-=bit;
        }
    }
}
