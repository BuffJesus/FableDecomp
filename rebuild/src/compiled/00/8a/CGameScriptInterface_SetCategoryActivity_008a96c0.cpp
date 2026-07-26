struct Key {
    int v;
    Key(int a);
    ~Key();
};

struct Map {
    char* lookup(Key* k);
};
extern Map g_map;

void __stdcall CGameScriptInterface_SetCategoryActivity(int category, bool active)
{
    Key temp(category);
    char* slot = g_map.lookup(&temp);
    *slot = (char)active;
}