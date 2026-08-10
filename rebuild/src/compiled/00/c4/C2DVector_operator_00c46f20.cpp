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