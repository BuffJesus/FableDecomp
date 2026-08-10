#include <cstdio>
struct Inner0 { char pad[0x160]; float scale; };
struct Inner4 { float val; };
struct CMouseCursor { Inner0* f0; Inner4* f4; };
static float GetVal(Inner4* s){ return s->val; }
static float model(CMouseCursor* self){ float v = GetVal(self->f4); return v * self->f0->scale; }
int main(){
    Inner0 a; a.scale = 3.0f;
    Inner4 b; b.val = 4.0f;
    CMouseCursor c; c.f0 = &a; c.f4 = &b;
    float r = model(&c);
    if (r != 12.0f) { printf("FAIL %f\n", r); return 1; }
    a.scale = 0.0f;
    if (model(&c) != 0.0f) { printf("FAIL2\n"); return 2; }
    printf("OK_00c44a90\n");
    return 0;
}