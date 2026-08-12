#include <cstdio>

struct SubInit {
    int marker;
    void Init();
};

struct CPhysicalPrimitiveInit {
    int   a;        // +0x00
    int   b;        // +0x04
    char  pad[0x14];// +0x08 .. +0x1b
    SubInit sub;    // +0x1c
    CPhysicalPrimitiveInit* GetDefaultVal();
};

void SubInit::Init() { marker = 0x1234; }

CPhysicalPrimitiveInit* CPhysicalPrimitiveInit::GetDefaultVal()
{
    a = 0;
    b = 0;
    sub.Init();
    return this;
}

int main() {
    CPhysicalPrimitiveInit obj;
    obj.a = 111;
    obj.b = 222;
    obj.sub.marker = 0;
    CPhysicalPrimitiveInit* r = obj.GetDefaultVal();
    if (r == &obj && obj.a == 0 && obj.b == 0 && obj.sub.marker == 0x1234) {
        printf("GETDEFAULTVAL_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}