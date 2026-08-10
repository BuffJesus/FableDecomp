#include <cstdio>

static void* callee(void* self, void* src, void* tmp)
{
    *(void**)self = *(void**)src;
    (void)tmp;
    return self;
}

static void* Ucopy_model(void* self, void* src)
{
    char localByte;
    return callee(self, src, &localByte);
}

int main()
{
    void* dst = 0;
    void* srcval = (void*)0xDEADBEEF;
    void* src = &srcval;
    void* r = Ucopy_model(&dst, src);
    int ok = 1;
    if (r != (void*)&dst) ok = 0;
    if (dst != (void*)0xDEADBEEF) ok = 0;

    void* srcval2 = (void*)0x12345678;
    void* src2 = &srcval2;
    void* dst2 = 0;
    Ucopy_model(&dst2, src2);
    if (dst2 != (void*)0x12345678) ok = 0;

    if (ok) printf("OK_005c13cd\n");
    else    printf("FAIL_005c13cd\n");
    return ok ? 0 : 1;
}