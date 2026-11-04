#pragma optimize("s", on)
// CNewFrontendGameComponent::UpdateInterpolation @ 0x0042DBFA (98 bytes).
// FableWin ?UpdateInterpolation@CNewFrontendGameComponent@@AAEXAAVCEngineInterpolationInfo@@@Z.
// The frontend runs at a notional 15 Hz: the interpolation weight is the
// time since the last update (+0xB8) in frames. Retail's multiplier is the
// float 0x416FFFFF (14.999999), one ulp under 15. Current / PausedCurrent get
// the new set, Last / PausedLast the previous one (m_LastInterp +0xCC).
#include "fable_game.h"

void CNewFrontendGameComponent::UpdateInterpolation(CEngineInterpolationInfo& info)
{
    CEngineInterpolationInfoSet set;
    set.WFInterpolate = (float)(GFGetTime() - m_fLastUpdateTime) * 14.999999f;
    info.Current = set;
    info.Last = m_LastInterp;
    info.PausedCurrent = set;
    info.PausedLast = m_LastInterp;
    m_LastInterp = set;
}
