#include <cstdio>

static int g_lastKey;

static int transform_key(long key) { return (int)key + 1; }

struct Sub {
    void* storage;
    void** index(int k) { g_lastKey = k; return &storage; }
};

struct World {
    char pad[0x14];
    Sub sub;
};

static void RegisterScriptedMapBrush(World* self, long key, void* brush)
{
    int k = transform_key(key);
    void** slot = self->sub.index(k);
    *slot = brush;
}

int main()
{
    World w;
    int dummyBrush = 0;
    RegisterScriptedMapBrush(&w, 41, &dummyBrush);
    if (w.sub.storage == &dummyBrush && g_lastKey == 42) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL\n");
    }
    return 0;
}