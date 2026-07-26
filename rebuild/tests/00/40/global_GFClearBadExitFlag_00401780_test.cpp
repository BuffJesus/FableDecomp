#include <cstdio>

static int g_calls = 0;

struct Obj {
    int a;
    int b;
    void Add(const char* name, int flag) { a = 1; g_calls++; (void)name; (void)flag; }
    void Set(const char* name, int flag) { b = 2; g_calls++; (void)name; (void)flag; }
    void Apply() { g_calls++; }
};

const char g_nameA[] = "A";
const char g_nameB[] = "B";

void GFClearBadExitFlag()
{
    Obj o;
    o.Add(g_nameA, 0);
    o.Set(g_nameB, 0);
    o.Apply();
}

int main()
{
    GFClearBadExitFlag();
    if (g_calls == 3) {
        std::printf("_global_00401780_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}