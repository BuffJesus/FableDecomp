#include <cstdio>

static int g_destroyed = 0;
static unsigned int g_flags = 0;

struct CMapResource {
    virtual void* Destroy(unsigned int flags);
};

void* CMapResource::Destroy(unsigned int flags)
{
    g_destroyed++;
    g_flags = flags;
    return this;
}

struct CEngineMap {
    CMapResource* res;
};

struct CEngineLandscapeRenderer {
    void DeleteMap(CEngineMap* map);
};

void CEngineLandscapeRenderer::DeleteMap(CEngineMap* map)
{
    CMapResource* r = map->res;
    if (r != 0)
        r->Destroy(1);
    map->res = 0;
}

int main()
{
    CEngineLandscapeRenderer rnd;
    CMapResource res;
    CEngineMap m;
    m.res = &res;

    rnd.DeleteMap(&m);
    if (g_destroyed != 1) { printf("FAIL destroyed=%d\n", g_destroyed); return 1; }
    if (g_flags != 1u) { printf("FAIL flags=%u\n", g_flags); return 1; }
    if (m.res != 0) { printf("FAIL res not null\n"); return 1; }

    // null case: no destroy, stays null
    CEngineMap m2;
    m2.res = 0;
    rnd.DeleteMap(&m2);
    if (g_destroyed != 1) { printf("FAIL destroyed2=%d\n", g_destroyed); return 1; }
    if (m2.res != 0) { printf("FAIL res2\n"); return 1; }

    printf("DELETEMAP_OK\n");
    return 0;
}