// CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate, retail 0x004C6010
// (FableWin ?ConvertInterpolateToBulletTimeInterpolate@CBulletTimeManager@@QBEMM@Z).
// With a slowdown multiplier of N, one world frame is spread over N render
// cycles: the frame's position within the current cycle plus the interpolate,
// both in units of 1/N. (FableWin's debug-only multiplier assert is compiled out.)
#include "engine/CBulletTimeManager.h"
#include "fable_world.h"

float CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate(float interpolate) const
{
    float multiplier = GetBulletTimeSlowdownMultiplier();
    float step = 1.0f / multiplier;
    long frameInCycle = (CWorld::GetFrame() - 1) % static_cast<long>(multiplier);
    float bulletInterpolate = frameInCycle * step;
    bulletInterpolate += step * interpolate;
    return bulletInterpolate;
}