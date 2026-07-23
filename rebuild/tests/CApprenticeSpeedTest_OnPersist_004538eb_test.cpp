#include <cstdio>

struct CApprenticeSpeedTest { int dummy; };

static int g_id = -1;
static float* g_pf = 0;
static unsigned char* g_pb = 0;

void __stdcall Persist_Serialize(int id, float* pf, unsigned char* pb)
{
    g_id = id;
    g_pf = pf;
    g_pb = pb;
    *pf = *pf + 1.0f;
}

CApprenticeSpeedTest* __fastcall CApprenticeSpeedTest_OnPersist(CApprenticeSpeedTest* self)
{
    float f = 0.0f;
    unsigned char b;
    Persist_Serialize(5, &f, &b);
    return self;
}

int main()
{
    CApprenticeSpeedTest obj;
    obj.dummy = 0x1234;
    CApprenticeSpeedTest* r = CApprenticeSpeedTest_OnPersist(&obj);
    if (r != &obj) { std::printf("FAIL: return not self\n"); return 1; }
    if (g_id != 5) { std::printf("FAIL: id=%d\n", g_id); return 1; }
    if (g_pf == 0 || g_pb == 0) { std::printf("FAIL: null ptr\n"); return 1; }
    std::printf("CApprenticeSpeedTest_004538eb_TEST PASS\n");
    return 0;
}