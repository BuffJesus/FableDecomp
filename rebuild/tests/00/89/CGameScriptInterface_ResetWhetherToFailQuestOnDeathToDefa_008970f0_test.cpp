#include <cstdio>

struct MapNode { int key; void* value; };

static MapNode g_end;
static MapNode g_found;
static int g_consumed = -1;
static int g_scenario = 0;

struct Iface { char pad[0x14]; void* sub; };

static void* GetSubObject(void* self) { return self; }
static unsigned char g_obj[0x100];
static void* Resolve(void* /*a*/) { return g_obj; }

static MapNode* MapLowerBound(void* /*self*/, int* /*key*/) {
    if (g_scenario == 1) return &g_end;      // node == end
    if (g_scenario == 2) { g_found.key = 4; return &g_found; } // key<=4
    if (g_scenario == 3) { g_found.key = 9; return &g_found; } // key>4 -> end
    return &g_end;
}
static void Consume(void* value) { g_consumed = (int)(long)value; }

static void Run(Iface* self) {
    void* a = GetSubObject(self->sub);
    unsigned char* eax = (unsigned char*)Resolve(a);
    if (!eax) return;
    if (eax[0x91] & 0x1) return;
    if (!(eax[0x20] & 0x10)) return;
    char* mapBase = (char*)eax + 0x44;
    int key = 4;
    MapNode* node = MapLowerBound(mapBase, &key);
    MapNode* endNode = *(MapNode**)(mapBase + 0x4);
    MapNode* sel;
    if (node == endNode) sel = endNode;
    else if (node->key <= 4) sel = node;
    else sel = endNode;
    Consume(sel->value);
}

int main() {
    Iface iface; iface.sub = 0;
    g_end.value = (void*)111;
    g_found.value = (void*)222;

    // set the end pointer inside g_obj at +0x44+4
    *(MapNode**)(g_obj + 0x44 + 4) = &g_end;
    g_obj[0x91] = 0; g_obj[0x20] = 0x10;

    g_scenario = 2; g_consumed = -1; Run(&iface);
    if (g_consumed != 222) { std::printf("FAIL found key<=4 got %d\n", g_consumed); return 1; }

    g_scenario = 3; g_consumed = -1; Run(&iface);
    if (g_consumed != 111) { std::printf("FAIL key>4 should use end got %d\n", g_consumed); return 1; }

    g_scenario = 1; g_consumed = -1; Run(&iface);
    if (g_consumed != 111) { std::printf("FAIL node==end got %d\n", g_consumed); return 1; }

    // gating: flag91 bit0 set -> early return
    g_obj[0x91] = 1; g_consumed = -1; Run(&iface);
    if (g_consumed != -1) { std::printf("FAIL flag91 gate\n"); return 1; }
    g_obj[0x91] = 0;

    g_obj[0x20] = 0; g_consumed = -1; Run(&iface);
    if (g_consumed != -1) { std::printf("FAIL flag20 gate\n"); return 1; }

    std::printf("CGameScriptInterface_008970f0_TEST PASS\n");
    return 0;
}