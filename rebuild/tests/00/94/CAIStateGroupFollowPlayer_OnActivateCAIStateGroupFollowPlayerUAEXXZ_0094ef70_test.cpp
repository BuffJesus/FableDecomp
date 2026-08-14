#include <cstdio>

// Behaviour model: OnActivate sets self->f1c = ObjMethod(GetObj(self), 2.6f) + g_bias
struct Obj { float scale; };
static float g_bias = 0.5f;

static Obj g_obj = { 3.0f };
static Obj* GetObj(void*) { return &g_obj; }
// model ObjMethod: return o->scale * v  (arbitrary but deterministic)
static float ObjMethod(Obj* o, float v) { return o->scale * v; }

struct Self {
    char pad[0x18];
    float f1c;
};

static void OnActivate(Self* self) {
    Obj* o = GetObj(self);
    self->f1c = ObjMethod(o, 2.6f) + g_bias;
}

int main() {
    Self s; s.f1c = -1.0f;
    OnActivate(&s);
    float expected = 3.0f * 2.6f + 0.5f;
    float d = s.f1c - expected;
    if (d < 0) d = -d;
    if (d < 1e-4f) printf("PARITY_OK\n");
    else printf("FAIL got=%f exp=%f\n", s.f1c, expected);
    return 0;
}