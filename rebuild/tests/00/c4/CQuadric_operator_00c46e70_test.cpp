#include <cstdio>

struct CQuadric {
    float f0;
    float f4;
};

CQuadric* __fastcall CQuadric_operator_add_eq(CQuadric* self, CQuadric* rhs, CQuadric* rhs2);

int main()
{
    CQuadric self = { 111.0f, 222.0f };
    CQuadric rhs  = { 1.5f, 2.5f };
    CQuadric rhs2 = { 10.0f, 20.0f };

    CQuadric_operator_add_eq(&self, &rhs, &rhs2);

    if (self.f0 == 11.5f && self.f4 == 22.5f)
        printf("00c46e70_TEST PASS\n");
    else
        printf("FAIL %f %f\n", self.f0, self.f4);
    return 0;
}