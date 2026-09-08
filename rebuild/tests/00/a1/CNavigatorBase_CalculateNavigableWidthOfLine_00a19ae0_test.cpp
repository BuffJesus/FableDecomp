#include "engine/CNavigatorBase.h"
#include <cstdio>

struct C3DVector { float x, y, z; };
struct CNavigatorBase_Methods : CNavigatorBase {
    void CalculateNavigableWidthOfLine(C3DVector const& a, C3DVector const& b,
                                       float& d, float& e);
};

int main()
{
    CNavigatorBase_Methods nav;
    C3DVector a = {1,2,3}, b = {4,5,6};
    float d = 9.0f, e = 8.0f;
    nav.CalculateNavigableWidthOfLine(a, b, d, e);
    if (d == 0.0f && e == 0.0f)
        printf("00a19ae0_TEST PASS\n");
    else
        printf("FAIL d=%f e=%f\n", d, e);
    return 0;
}
