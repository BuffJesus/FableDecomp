#include <cstdio>

struct SVec2 { int a; int b; };

struct CEngineLightingManager {
    char pad[0x18];
    int  mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* objA;     // +0x24
    void* objB;     // +0x28
};

static int g_markerCalls = 0;
static int g_fetchRet[2] = {111, 222};
static void* g_lastApplyOut = 0;
static void* g_lastApplySelf = 0;

void __stdcall Marker(const char* s) { (void)s; g_markerCalls++; }

struct ObjA { int* Fetch(SVec2* out); };
struct ObjB { void Apply(SVec2* p); };

int* ObjA::Fetch(SVec2* out) { (void)out; return g_fetchRet; }
void ObjB::Apply(SVec2* p) { g_lastApplySelf = this; g_lastApplyOut = p; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, SVec2* out)
{
    SVec2 tmp;
    Marker((const char*)0x122d70e);
    switch (self->mode) {
    case 2: {
        int* r = ((ObjA*)self->objA)->Fetch(&tmp);
        out->a = r[0];
        out->b = r[1];
        break;
    }
    case 3:
        ((ObjB*)self->objB)->Apply(out);
        break;
    }
}

int main() {
    CEngineLightingManager obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;
    obj.objA = (void*)0x1000;
    obj.objB = (void*)0x2000;
    SVec2 out;

    obj.mode = 2; out.a = out.b = -1;
    CEngineLightingManager_UpdateShadowScene(&obj, 0, &out);
    if (out.a != 111 || out.b != 222) { std::printf("FAIL mode2\n"); return 1; }

    obj.mode = 3; g_lastApplyOut = 0;
    CEngineLightingManager_UpdateShadowScene(&obj, 0, &out);
    if (g_lastApplyOut != &out || g_lastApplySelf != (void*)0x2000) { std::printf("FAIL mode3\n"); return 1; }

    obj.mode = 5; out.a = 77;
    CEngineLightingManager_UpdateShadowScene(&obj, 0, &out);
    if (out.a != 77) { std::printf("FAIL mode5\n"); return 1; }

    if (g_markerCalls != 3) { std::printf("FAIL marker\n"); return 1; }

    std::printf("CEngineLightingManager_004568bc_TEST PASS\n");
    return 0;
}