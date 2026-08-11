#include <cstdio>
#include <cstring>

struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

char kLayoutName[] = "LightsBones";

struct CEngineVSConstantLayoutLightsBones {
    int x;
    CCharString PeekName();
};

static const char* g_last_s;
static int g_last_n;

CCharString::CCharString(const char* s, int n)
{
    d = 0x1234;
    g_last_s = s;
    g_last_n = n;
}

int main()
{
    CEngineVSConstantLayoutLightsBones obj; obj.x = 7;
    CCharString r = obj.PeekName();
    if (r.d == 0x1234 && g_last_n == -1 && strcmp(g_last_s, "LightsBones") == 0) {
        printf("00bdb2f0_TEST PASS\n");
    } else {
        printf("FAIL d=%d n=%d\n", r.d, g_last_n);
    }
    return 0;
}