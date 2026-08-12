#include <cstdio>
#include <cstring>

struct CNavigatorExternalInterface { int dummy; };

static int g_ctorCalls = 0;

struct CObstacleNavigatorExternalInterface
{
    char data[0x14];
    CObstacleNavigatorExternalInterface() {}
    CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& src)
    {
        g_ctorCalls++;
        memcpy(data, src.data, sizeof(data));
    }
    CNavigatorExternalInterface* Clone();
};

CNavigatorExternalInterface* CObstacleNavigatorExternalInterface::Clone()
{
    return (CNavigatorExternalInterface*)new CObstacleNavigatorExternalInterface(*this);
}

int main()
{
    CObstacleNavigatorExternalInterface src;
    for (int i = 0; i < 0x14; ++i) src.data[i] = (char)(i + 1);

    CNavigatorExternalInterface* r = src.Clone();
    int ok = 1;
    if (r == 0) ok = 0;
    if (g_ctorCalls != 1) ok = 0;

    CObstacleNavigatorExternalInterface* rp = (CObstacleNavigatorExternalInterface*)r;
    for (int i = 0; i < 0x14; ++i)
        if (rp->data[i] != (char)(i + 1)) ok = 0;

    // Independent clone -> distinct storage.
    if (ok && rp == &src) ok = 0;

    if (ok) printf("CLONE_OK\n");
    else printf("CLONE_FAIL\n");
    return ok ? 0 : 1;
}