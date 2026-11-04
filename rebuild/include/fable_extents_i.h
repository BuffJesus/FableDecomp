#pragma once
class C2DExtentsI
{
public:
    C2DExtentsI() {}
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
    long X;
    long Y;
};

class C3DExtentsI : public C2DExtentsI
{
public:
    C3DExtentsI() {}
    C3DExtentsI(long x, long y, long z) : C2DExtentsI(x, y), Z(z) {}
    long Z;
};

