#include <stdio.h>

static const float k_CrierScale = 0.5f;

struct CTCVillage
{
    char pad0;
    char crier;
};

void __fastcall CTCVillage_ResetCrier(CTCVillage* self, int, float x)
{
    self->crier = (char)(long)(x * k_CrierScale);
}

int main()
{
    CTCVillage v;
    v.pad0 = 0x7F;
    v.crier = 0;

    // 10.0 * 0.5 = 5.0 -> (long)5 -> (char)5
    CTCVillage_ResetCrier(&v, 0, 10.0f);
    bool ok = (v.crier == 5);

    // 7.0 * 0.5 = 3.5 -> truncated (long)3 -> (char)3
    CTCVillage_ResetCrier(&v, 0, 7.0f);
    ok = ok && (v.crier == 3);

    // negative: -9.0 * 0.5 = -4.5 -> (long)-4 -> (char)-4
    CTCVillage_ResetCrier(&v, 0, -9.0f);
    ok = ok && (v.crier == (char)-4);

    // ensure pad0 untouched
    ok = ok && (v.pad0 == 0x7F);

    if (ok) printf("RESETCRIER_OK\n");
    else    printf("RESETCRIER_FAIL crier=%d\n", (int)v.crier);
    return ok ? 0 : 1;
}