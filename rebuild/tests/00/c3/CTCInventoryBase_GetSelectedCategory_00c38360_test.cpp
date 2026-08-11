#include <cstdio>

struct CCategory;

struct CTCInventoryBase {
    CCategory* f0;
    CCategory* f4;
};

bool __fastcall CTCInventoryBase_GetSelectedCategory(CTCInventoryBase* self, CTCInventoryBase* other);

int main()
{
    CCategory* A = (CCategory*)0x1000;
    CCategory* B = (CCategory*)0x2000;

    CTCInventoryBase s0; s0.f0 = A; s0.f4 = B;
    CTCInventoryBase o0; o0.f0 = A; o0.f4 = B;   // both equal -> true
    CTCInventoryBase o1; o1.f0 = A; o1.f4 = A;   // f4 differ -> false
    CTCInventoryBase o2; o2.f0 = B; o2.f4 = B;   // f4 same, f0 differ -> false

    bool ok = true;
    ok = ok && (CTCInventoryBase_GetSelectedCategory(&s0, &o0) == true);
    ok = ok && (CTCInventoryBase_GetSelectedCategory(&s0, &o1) == false);
    ok = ok && (CTCInventoryBase_GetSelectedCategory(&s0, &o2) == false);

    if (ok) printf("GETSELCAT_OK\n");
    else    printf("GETSELCAT_FAIL\n");
    return ok ? 0 : 1;
}