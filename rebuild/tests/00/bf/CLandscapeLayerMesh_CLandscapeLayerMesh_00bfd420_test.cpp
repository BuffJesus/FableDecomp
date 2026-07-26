#include <cstdio>
#include <cstring>

struct CLandscapeLayerMesh {
    void* vtbl;
    void* pParam;
    int f08; int f0c; int f10; int f14; int f18; int f1c;
    int f20; int f24; int f28; int f2c;
    short f30; short f32;
    char pad34[0x40-0x34];
    int f40;
    char f44; char pad45[3];
    int f48; int f4c; int f50;
    short f54; short f56;
    int f58; int f5c;
};

CLandscapeLayerMesh* __fastcall CLandscapeLayerMesh_CLandscapeLayerMesh(CLandscapeLayerMesh* self, void* edx, void* param) {
    self->pParam = param;
    int z = 0;
    self->vtbl = (void*)0x12a8334;
    self->f08 = z;
    self->f0c = z;
    self->f30 = (short)z;
    self->f32 = (short)z;
    self->f40 = z;
    self->f44 = 1;
    self->f48 = z;
    self->f4c = z;
    self->f50 = z;
    self->f54 = (short)z;
    self->f56 = (short)z;
    self->f58 = z;
    self->f5c = z;
    self->f10 = z;
    self->f20 = z;
    self->f14 = z;
    self->f24 = z;
    self->f18 = z;
    self->f28 = z;
    self->f1c = z;
    self->f2c = z;
    return self;
}

int main() {
    CLandscapeLayerMesh obj;
    std::memset(&obj, 0xAB, sizeof(obj));
    int dummy = 12345;
    CLandscapeLayerMesh* r = CLandscapeLayerMesh_CLandscapeLayerMesh(&obj, 0, &dummy);
    if (r != &obj) { std::printf("FAIL return\n"); return 1; }
    if (obj.vtbl != (void*)0x12a8334) { std::printf("FAIL vtbl\n"); return 1; }
    if (obj.pParam != &dummy) { std::printf("FAIL pParam\n"); return 1; }
    if (obj.f44 != 1) { std::printf("FAIL f44\n"); return 1; }
    if (obj.f08 || obj.f0c || obj.f10 || obj.f5c || obj.f30 || obj.f54) { std::printf("FAIL zero\n"); return 1; }
    std::printf("CLandscapeLayerMesh_00bfd420_TEST PASS\n");
    return 0;
}