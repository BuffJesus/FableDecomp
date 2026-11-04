// CEngineInterpolationInfo::CEngineInterpolationInfo @ 0x0042FA30 (158 bytes).
// FableWin ??0CEngineInterpolationInfo@@QAE@XZ. Every set zero-constructs;
// the three-entry InterpolationSetList (+0x4C) then mirrors the Current,
// PausedCurrent and BulletTimeCurrent sets.
#include "fable_game.h"

CEngineInterpolationInfo::CEngineInterpolationInfo()
    : WFServerCurrentTime(0)
{
    InterpolationSetList[0] = Current;
    InterpolationSetList[1] = PausedCurrent;
    InterpolationSetList[2] = BulletTimeCurrent;
}