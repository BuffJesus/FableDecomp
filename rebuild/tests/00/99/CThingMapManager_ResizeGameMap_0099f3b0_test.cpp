#include <cstdio>

struct C2DBoxI { int a, b, c, d; };

struct CThingMap {
    void ResizeMap(long v, C2DBoxI* box);
};

struct CThingMapManager {
    CThingMap* map;
    void Prepare();
    void ResizeGameMap(long v, C2DBoxI* box);
};

static int g_prepare = 0;
static long g_v = 0;
static C2DBoxI* g_box = 0;

void CThingMapManager::Prepare() { g_prepare++; }
void CThingMap::ResizeMap(long v, C2DBoxI* box) { g_v = v; g_box = box; }

int main()
{
    CThingMap m;
    CThingMapManager mgr;
    mgr.map = &m;
    C2DBoxI box; box.a = 1; box.b = 2; box.c = 3; box.d = 4;

    mgr.ResizeGameMap(42, &box);

    if (g_prepare == 1 && g_v == 42 && g_box == &box)
        printf("RESIZE_OK\n");
    else
        printf("FAIL\n");
    return (g_prepare == 1 && g_v == 42 && g_box == &box) ? 0 : 1;
}