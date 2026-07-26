#include <cstdio>

static char g_storage[4];
static int g_last_key;

struct Key {
    int v;
    Key(int a) { v = a; }
    ~Key() {}
};

struct Map {
    char* lookup(Key* k) { g_last_key = k->v; return g_storage; }
};
Map g_map;

void __stdcall CGameScriptInterface_SetCategoryActivity(int category, bool active)
{
    Key temp(category);
    char* slot = g_map.lookup(&temp);
    *slot = (char)active;
}

int main(){
    g_storage[0] = 0;
    CGameScriptInterface_SetCategoryActivity(7, true);
    if(g_last_key != 7){ std::printf("FAIL key=%d\n", g_last_key); return 1; }
    if(g_storage[0] != 1){ std::printf("FAIL stored=%d\n", (int)g_storage[0]); return 1; }
    CGameScriptInterface_SetCategoryActivity(9, false);
    if(g_last_key != 9 || g_storage[0] != 0){ std::printf("FAIL second\n"); return 1; }
    std::printf("CGameScriptInterface_008a96c0_TEST PASS\n");
    return 0;
}