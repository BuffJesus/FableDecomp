#include <cstdio>

struct MapNode {
    int key;
    void* value;
};

struct Map {
    void* comp;
    MapNode* head;
};

struct Env;

struct Env {
    void* vt;
    unsigned char _p04[0x20];
    unsigned char flag24;
    unsigned char _p25[0x1f];
    Map map;
    unsigned char _p4c[0x45];
    unsigned char flag91;
};

struct IFace {
    void** vt;
};

// test stubs
static MapNode g_node;
static MapNode g_end;
static bool g_evalResult = true;
static void* g_lastEvalVal = 0;
static MapNode* g_lbReturn = 0;

struct KeyArg { int* p; };
extern "C" MapNode* __fastcall Map_LowerBound(Map* self, KeyArg k) {
    (void)self; (void)k;
    return g_lbReturn;
}
extern "C" bool __fastcall Node_Eval(void* val) {
    g_lastEvalVal = val;
    return g_evalResult;
}

typedef Env* (__fastcall *GetEnvFn)(IFace*);

static Env* g_env = 0;
static Env* __fastcall getEnv(IFace* self) { (void)self; return g_env; }

bool __stdcall CGameScriptInterface_IsSleepingTime(IFace* self)
{
    Env* e = ((GetEnvFn)self->vt[0x2c/4])(self);
    if (e != 0 && (e->flag91 & 1) == 0 && (e->flag24 & 4) != 0) {
        Map* m = &e->map;
        int key = 0x22;
        KeyArg ka; ka.p = &key;
        MapNode* it = Map_LowerBound(m, ka);
        MapNode* end = m->head;
        if (it == end) it = end;
        else if (it->key > 0x22) it = end;
        return Node_Eval(it->value);
    }
    return false;
}

int main() {
    void* vtbuf[16];
    for (int i=0;i<16;i++) vtbuf[i]=0;
    vtbuf[0x2c/4] = (void*)&getEnv;
    IFace iface; iface.vt = vtbuf;

    // Case 1: null env -> false
    g_env = 0;
    if (CGameScriptInterface_IsSleepingTime(&iface) != false) { std::printf("FAIL null\n"); return 1; }

    // Build an env
    static Env env;
    for (unsigned i=0;i<sizeof(env);i++) ((unsigned char*)&env)[i]=0;
    env.map.head = &g_end;
    g_env = &env;

    // Case 2: flag91 bit0 set -> false
    env.flag91 = 1; env.flag24 = 4;
    if (CGameScriptInterface_IsSleepingTime(&iface) != false) { std::printf("FAIL flag91\n"); return 1; }

    // Case 3: flag24 bit2 clear -> false
    env.flag91 = 0; env.flag24 = 0;
    if (CGameScriptInterface_IsSleepingTime(&iface) != false) { std::printf("FAIL flag24\n"); return 1; }

    // Case 4: found node with key <= 0x22 -> eval node->value
    env.flag91 = 0; env.flag24 = 4;
    g_node.key = 0x10; g_node.value = (void*)0xABCD;
    g_lbReturn = &g_node;
    g_evalResult = true;
    if (CGameScriptInterface_IsSleepingTime(&iface) != true) { std::printf("FAIL found\n"); return 1; }
    if (g_lastEvalVal != (void*)0xABCD) { std::printf("FAIL evalval\n"); return 1; }

    // Case 5: node key > 0x22 -> use end, eval end->value
    g_node.key = 0x30;
    g_end.value = (void*)0x1234;
    g_lastEvalVal = 0;
    g_evalResult = false;
    if (CGameScriptInterface_IsSleepingTime(&iface) != false) { std::printf("FAIL endpath\n"); return 1; }
    if (g_lastEvalVal != (void*)0x1234) { std::printf("FAIL endval\n"); return 1; }

    std::printf("CGameScriptInterface_00896210_TEST PASS\n");
    return 0;
}