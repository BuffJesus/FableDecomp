#include <math.h>
#include <stdio.h>

extern "C" double __stdcall x87_Exp_0066f9f0(double value);

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

static int NearlyEqual(double left, double right)
{
    return fabs(left - right) < 1.0e-12;
}

int main()
{
    int ok = 1;
    const double ln2 = 0.69314718055994530942;

    ok &= Check(x87_Exp_0066f9f0(0.0) == 1.0,
                "exp(0) must equal one");
    ok &= Check(NearlyEqual(x87_Exp_0066f9f0(ln2), 2.0),
                "exp(ln(2)) must equal two");
    ok &= Check(NearlyEqual(x87_Exp_0066f9f0(-ln2), 0.5),
                "exp(-ln(2)) must equal one half");
    ok &= Check(NearlyEqual(x87_Exp_0066f9f0(1.0), exp(1.0)),
                "exp(1) must agree with the runtime result");

    if (!ok)
        return 1;

    puts("PASS x87 exponential helper 0x0066F9F0");
    return 0;
}
