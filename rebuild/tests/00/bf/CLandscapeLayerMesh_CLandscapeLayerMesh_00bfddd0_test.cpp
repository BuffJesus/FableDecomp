#include <cstdio>

struct CLandscapeLayerMesh {
    void* vtbl;
    int f04, f08, f0c, f10, f14, f18, f1c, f20, f24, f28, f2c, f30, f34, f38, f3c, f40;
    short f44, f46;
    char f48;
    CLandscapeLayerMesh* CLandscapeLayerMesh_ctor(int arg);
};

void* CLandscapeLayerMesh_vtbl;

CLandscapeLayerMesh* CLandscapeLayerMesh::CLandscapeLayerMesh_ctor(int arg) {
    this->f40 = arg;
    this->vtbl = &CLandscapeLayerMesh_vtbl;
    this->f3c = 0;
    this->f44 = 0;
    this->f46 = 0;
    this->f04 = 0;
    this->f08 = 0;
    this->f0c = 0;
    this->f14 = 0;
    this->f18 = 0;
    this->f1c = 0;
    this->f28 = 0;
    this->f34 = 0;
    this->f38 = 0;
    this->f48 = 0;
    this->f30 = 0;
    this->f2c = 0;
    this->f20 = 0;
    this->f24 = 0;
    return this;
}

int main() {
    CLandscapeLayerMesh obj;
    for (char* p = (char*)&obj; p < (char*)&obj + sizeof(obj); ++p) *p = (char)0xAB;
    CLandscapeLayerMesh* r = obj.CLandscapeLayerMesh_ctor(0x12345678);
    if (r != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (obj.f40 != 0x12345678) { std::printf("FAIL: arg\n"); return 1; }
    if (obj.vtbl != &CLandscapeLayerMesh_vtbl) { std::printf("FAIL: vtbl\n"); return 1; }
    if (obj.f3c != 0 || obj.f44 != 0 || obj.f46 != 0 || obj.f04 != 0 ||
        obj.f08 != 0 || obj.f0c != 0 || obj.f14 != 0 || obj.f18 != 0 ||
        obj.f1c != 0 || obj.f28 != 0 || obj.f34 != 0 || obj.f38 != 0 ||
        obj.f48 != 0 || obj.f30 != 0 || obj.f2c != 0 || obj.f20 != 0 ||
        obj.f24 != 0) { std::printf("FAIL: zero fields\n"); return 1; }
    std::printf("CLandscapeLayerMesh_00bfddd0_TEST PASS\n");
    return 0;
}