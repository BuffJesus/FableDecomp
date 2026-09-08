#include "engine/CNavigatorFlyer.h"

struct CNavigatorFlyer_Methods : CNavigatorFlyer {
    virtual void slot0(); virtual void slot1(); virtual void slot2(); virtual void slot3();
    virtual void MoveToNavLineOfSight();
    void MoveToNavLineOfSightProximity();
};
void CNavigatorFlyer_Methods::MoveToNavLineOfSightProximity() { MoveToNavLineOfSight(); }
