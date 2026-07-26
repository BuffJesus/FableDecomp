// CGameScriptInterface::IsSleepingTime @ 00896210
struct MapNode {
    int key;      // +0x0
    void* value;  // +0x4
};

struct Map {
    void* comp;      // +0x0
    MapNode* head;   // +0x4
};

struct Env {
    void* vt;                    // +0x0
    unsigned char _p04[0x20];    // to 0x24
    unsigned char flag24;        // +0x24
    unsigned char _p25[0x1f];    // to 0x44
    Map map;                     // +0x44
    unsigned char _p4c[0x45];    // to 0x91
    unsigned char flag91;        // +0x91
};

struct IFace {
    void** vt;
};

struct KeyArg { int* p; };
extern "C" MapNode* __fastcall Map_LowerBound(Map* self, KeyArg k);
extern "C" bool __fastcall Node_Eval(void* val);

typedef Env* (__fastcall *GetEnvFn)(IFace*);

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