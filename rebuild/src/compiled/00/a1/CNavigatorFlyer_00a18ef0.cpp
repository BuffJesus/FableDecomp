// CNavigatorFlyer (retail vtable 0x0129CB54). Out-of-line overrides 0x00A18EF0..0x00A18F30
// (slots 8, 5, 11, 14, 15; Ego_r.pdb module order MoveToPos, GetNearestNavigablePosTo,
// MoveToNavLineOfSightProximity, IsNewDestinationGoingToCauseRestart, CalculateRouteFromTo,
// InitialiseFromPreCalculatedRoute) and the constructor 0x00A18F40 (CNavigatorBase ctor
// 0x00A47110 with navigator type 4, then the vtable store).
// The constructor emits the vtable, and with it the inline virtuals as COMDATs, in retail
// order after the out-of-line bodies: CClass::Save 0x00A19AB0, CNavigatorBase::CreateLineList
// 0x00A19AC0 / Interrupt 0x00A19AD0 / CalculateNavigableWidthOfLine 0x00A19AE0,
// GetCurrentDestination 0x00A19B00, UpdateLastPosition 0x00A19B10, ??_G 0x00A19B20.
#include "fable_navigator.h"

bool CNavigatorFlyer::GetNearestNavigablePosTo(const C3DVector&, float, C3DVector&) const
{
    return false;
}

ENavigatorReturn CNavigatorFlyer::MoveToNavLineOfSightProximity(const C3DVector& from, const C3DVector& to,
                                                                 float proximity, float speed,
                                                                 C3DVector& movement, bool a, bool b,
                                                                 bool c, bool d, bool e, bool f, float g)
{
    return MoveToPosProximity(from, to, proximity, speed, movement, a, b, c, d, e, f, g);
}

bool CNavigatorFlyer::IsNewDestinationGoingToCauseRestart(const C3DVector&, float) const
{
    return true;
}

void CNavigatorFlyer::CalculateRouteFromTo(const C3DVector&, const C3DVector&, CNavigatorRoute&)
{
}

void CNavigatorFlyer::InitialiseFromPreCalculatedRoute(const C3DVector&, const CNavigatorRoute&,
                                                       const C3DVector&)
{
}

CNavigatorFlyer::CNavigatorFlyer(CNavigatorManager& manager, const CNavigatorExternalInterface& external)
    : CNavigatorBase(manager, 4, external)
{
}
