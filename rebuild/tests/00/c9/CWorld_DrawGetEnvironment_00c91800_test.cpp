#include <cstdio>
struct CIDrawEnvironment;
struct CWorld {
    void* pad0;
    void* pad4;
    CIDrawEnvironment* env8;
};
static CIDrawEnvironment* model(CWorld* self) { return self->env8; }
int main() {
    CWorld w;
    w.pad0 = (void*)0x11111111;
    w.pad4 = (void*)0x22222222;
    w.env8 = (CIDrawEnvironment*)0xDEADBEEF;
    if (model(&w) != (CIDrawEnvironment*)0xDEADBEEF) { printf("FAIL\n"); return 1; }
    w.env8 = (CIDrawEnvironment*)0;
    if (model(&w) != (CIDrawEnvironment*)0) { printf("FAIL\n"); return 1; }
    printf("OK_00c91800\n");
    return 0;
}