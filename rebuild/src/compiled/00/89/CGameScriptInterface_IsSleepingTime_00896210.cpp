// CGameScriptInterface::IsSleepingTime @ 00896210
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

struct MapNode {
    int key;
    void* value;
};

struct Map {
    void* comp;
    MapNode* head;
};

struct Env {
    void* vt;
    unsigned char _p04[0x20];
    unsigned char flag24;
    unsigned char _p25[0x1f];
    Map map;
    unsigned char _p4c[0x45];
    unsigned char flag91;
};

struct KeyArg { int* p; };
extern "C" MapNode* __fastcall Map_LowerBound(Map* self, KeyArg k);
extern "C" bool __fastcall Node_Eval(void* val);

typedef Env* (__fastcall *GetEnvFn)(CScriptThing*);

// The retail body does not access its CGameScriptInterface `this`; this ABI-normalized
// free function therefore exposes only the one stack argument used by the body.
bool __stdcall CGameScriptInterface_IsSleepingTime(CScriptThing* scriptThing)
{
    Env* e = ((GetEnvFn)((void**)scriptThing->__vftable)[0x2c/4])(scriptThing);
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
