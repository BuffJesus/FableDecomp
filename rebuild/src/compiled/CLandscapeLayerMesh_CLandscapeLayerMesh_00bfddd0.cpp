struct CLandscapeLayerMesh {
    void* vtbl;        // +0x00
    int f04;           // +0x04
    int f08;           // +0x08
    int f0c;           // +0x0c
    int f10;           // +0x10
    int f14;           // +0x14
    int f18;           // +0x18
    int f1c;           // +0x1c
    int f20;           // +0x20
    int f24;           // +0x24
    int f28;           // +0x28
    int f2c;           // +0x2c
    int f30;           // +0x30
    int f34;           // +0x34
    int f38;           // +0x38
    int f3c;           // +0x3c
    int f40;           // +0x40
    short f44;         // +0x44
    short f46;         // +0x46
    char f48;          // +0x48
    CLandscapeLayerMesh* CLandscapeLayerMesh_ctor(int arg);
};

extern void* CLandscapeLayerMesh_vtbl;

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