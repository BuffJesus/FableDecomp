#include <cstdio>

struct CQuadric {
    float a;
    float b;
    void OperatorPlusEq(CQuadric* other) {
        a = other->a + a;
        b = other->b + b;
    }
};

int main()
{
    CQuadric d; d.a = 1.5f; d.b = -2.0f;
    CQuadric s; s.a = 0.5f; s.b = 3.0f;
    d.OperatorPlusEq(&s);
    if (d.a != 2.0f) { printf("FAIL a=%f\n", d.a); return 1; }
    if (d.b != 1.0f) { printf("FAIL b=%f\n", d.b); return 1; }
    CQuadric z; z.a = 0.0f; z.b = 0.0f;
    CQuadric z2; z2.a = 0.0f; z2.b = 0.0f;
    z.OperatorPlusEq(&z2);
    if (z.a != 0.0f || z.b != 0.0f) { printf("FAIL zero\n"); return 1; }
    CQuadric n; n.a = -5.0f; n.b = 10.0f;
    CQuadric m; m.a = 2.0f; m.b = -4.0f;
    n.OperatorPlusEq(&m);
    if (n.a != -3.0f || n.b != 6.0f) { printf("FAIL neg\n"); return 1; }
    printf("OK_00c46da0\n");
    return 0;
}