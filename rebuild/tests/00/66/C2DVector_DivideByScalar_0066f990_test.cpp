#include <stdio.h>

extern "C" void* __fastcall
C2DVector_DivideByScalar_0066f990(
    void* vector, void*, float divisor);

extern "C" float C2DVector_DivideByScalar_0066f990_Unit = 1.0f;

struct C2DVector
{
    float x;
    float y;
};

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    C2DVector vector;
    void* returned;

    vector.x = 6.0f;
    vector.y = -9.0f;
    returned = C2DVector_DivideByScalar_0066f990(&vector, 0, 3.0f);
    ok &= Check(returned == &vector,
                "divide helper must return this");
    ok &= Check(vector.x == 2.0f && vector.y == -3.0f,
                "positive divisor must scale both components");

    vector.x = -8.0f;
    vector.y = 4.0f;
    returned = C2DVector_DivideByScalar_0066f990(&vector, 0, -2.0f);
    ok &= Check(returned == &vector,
                "returned pointer must remain stable");
    ok &= Check(vector.x == 4.0f && vector.y == -2.0f,
                "negative divisor must preserve divide semantics");

    vector.x = 1.25f;
    vector.y = -0.5f;
    C2DVector_DivideByScalar_0066f990(&vector, 0, 1.0f);
    ok &= Check(vector.x == 1.25f && vector.y == -0.5f,
                "unit divisor must preserve the vector");

    if (!ok)
        return 1;

    puts("PASS C2DVector divide-by-scalar helper 0x0066F990");
    return 0;
}
