#include <cstdio>

struct SomeGlobalHolder {
    char pad[0x958];
    int* pHeroId;
};

struct CMarkerInfo {
    int field0;
    int field4;
    int markerId;
    bool IsHeroMarker();
};

static int g_heroId;
static SomeGlobalHolder g_holder;

SomeGlobalHolder* GetGlobalHolder(void) { return &g_holder; }

bool CMarkerInfo::IsHeroMarker()
{
    int* p = GetGlobalHolder()->pHeroId;
    return this->markerId == *p;
}

int main()
{
    g_heroId = 42;
    g_holder.pHeroId = &g_heroId;

    CMarkerInfo a;
    a.markerId = 42;
    CMarkerInfo b;
    b.markerId = 7;

    bool ra = a.IsHeroMarker();
    bool rb = b.IsHeroMarker();

    if (ra && !rb) {
        printf("PASS_ISHEROMARKER\n");
        return 0;
    }
    printf("FAIL %d %d\n", (int)ra, (int)rb);
    return 1;
}