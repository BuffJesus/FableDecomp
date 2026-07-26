#include <cstdio>

struct Sub { char pad[0x20]; };

static int g_ret = 1;
static int* g_lastOut = 0;
static int* g_lastIn = 0;

bool __fastcall Sub_Compute(Sub* self, int* edxreg, int* outA, int* inB)
{
    g_lastOut = outA;
    g_lastIn = inB;
    *outA = 42;
    return g_ret != 0;
}

bool __fastcall CTCLook_IsTurningBody(void* self, int edxpad, int arg)
{
    int local0[2];
    return Sub_Compute((Sub*)((char*)self + 0x20), &local0[0], &local0[0], &arg);
}

int main()
{
    char obj[0x40];
    g_ret = 1;
    bool r = CTCLook_IsTurningBody(obj, 0, 7);
    if (r != true) { std::printf("FAIL r\n"); return 1; }
    if (*g_lastIn != 7) { std::printf("FAIL param\n"); return 1; }
    g_ret = 0;
    if (CTCLook_IsTurningBody(obj, 0, 3) != false) { std::printf("FAIL r2\n"); return 1; }
    std::printf("CTCLook_00638180_TEST PASS\n");
    return 0;
}