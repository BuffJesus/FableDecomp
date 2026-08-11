#include <cstdio>

struct GPBOut {
    int   f0;
    float f4;
    int   f8;
};

struct CTCGatherPointBuilding {
    int            f0;
    volatile float f4;
    int            f8;
    void Init(GPBOut *out);
};

void CTCGatherPointBuilding::Init(GPBOut *out)
{
    float t = this->f4;
    int   u = this->f0;
    out->f4 = t;
    out->f0 = u;
    out->f8 = 0;
}

int main()
{
    CTCGatherPointBuilding s;
    s.f0 = 0x12345678;
    s.f4 = 3.5f;
    s.f8 = 999;

    GPBOut o;
    o.f0 = -1; o.f4 = -1.0f; o.f8 = -1;

    s.Init(&o);

    if (o.f0 == 0x12345678 && o.f4 == 3.5f && o.f8 == 0)
        printf("00c46eb0_TEST PASS\n");
    else
        printf("FAIL f0=%d f4=%f f8=%d\n", o.f0, o.f4, o.f8);
    return 0;
}