#include <cstdio>

struct C2DVector {
    float x;
    float y;
};

C2DVector * __fastcall C2DVector_operator_mul_assign(C2DVector *self, C2DVector *rhs, float s)
{
    float ny = s * rhs->y;
    self->x = s * rhs->x;
    self->y = ny;
    return self;
}

int main()
{
    C2DVector self;
    self.x = 1.0f; self.y = 1.0f;
    C2DVector rhs;
    rhs.x = 3.0f; rhs.y = 5.0f;
    C2DVector *r = C2DVector_operator_mul_assign(&self, &rhs, 2.0f);
    if (r == &self && self.x == 6.0f && self.y == 10.0f)
        printf("00c46f20_TEST PASS\n");
    else
        printf("FAIL %f %f\n", self.x, self.y);
    return 0;
}