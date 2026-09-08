#include "engine/CNavigatorBase.h"

struct C3DVector { float x, y, z; };

struct CNavigatorBase_Methods : CNavigatorBase {
    void CalculateNavigableWidthOfLine(C3DVector const& a, C3DVector const& b,
                                       float& d, float& e);
};

void CNavigatorBase_Methods::CalculateNavigableWidthOfLine(C3DVector const& a,
        C3DVector const& b, float& d, float& e)
{
    (void)a; (void)b;
    e = 0.0f;
    d = 0.0f;
}
