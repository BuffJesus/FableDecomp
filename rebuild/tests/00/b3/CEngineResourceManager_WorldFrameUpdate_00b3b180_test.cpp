#include <stdio.h>

static int g_called = 0;
static void* g_this = 0;

struct Sub { char pad[4]; };
struct Inner { char pad[0x1ec]; Sub sub; };
struct CEngineResourceManager { char pad[0x10]; Inner* field10; };

void model_update(Sub* self) { g_called++; g_this = self; }

void model_wfu(CEngineResourceManager* self, bool b) {
    Inner* p = self->field10;
    if (p) model_update(&p->sub);
}

int main() {
    // null branch
    CEngineResourceManager m0;
    m0.field10 = 0;
    g_called = 0;
    model_wfu(&m0, false);
    if (g_called != 0) { printf("FAIL null\n"); return 1; }

    // non-null branch
    Inner inner;
    CEngineResourceManager m1;
    m1.field10 = &inner;
    g_called = 0; g_this = 0;
    model_wfu(&m1, true);
    if (g_called != 1) { printf("FAIL call\n"); return 1; }
    if (g_this != (void*)((char*)&inner + 0x1ec)) { printf("FAIL this\n"); return 1; }

    printf("OK_00b3b180\n");
    return 0;
}