#include <cstdio>

struct CCamera { int tag; float m; void* a; void* b; char rest[0x30]; };
struct CFieldA { int id; };
struct CFieldB { int id; };

struct CGameCameraManager {
    void*   vptr;            // +0x00
    CFieldA field4;          // +0x04
    char    pad8[0x34];      // +0x08 .. +0x3b
    CFieldB field3c;         // +0x3c

    // Behaviour model of the 0x6978d0 helper member: builds a camera from
    // (m, &field4, &field3c).
    CCamera Helper6978d0(float mm, CFieldA* a, CFieldB* b) const {
        CCamera c;
        c.tag = 0xCA;
        c.m   = mm;
        c.a   = a;
        c.b   = b;
        return c;
    }

    // The reconstructed forwarder.
    CCamera GetRenderCamera(float mm) const {
        return Helper6978d0(mm, (CFieldA*)&field4, (CFieldB*)&field3c);
    }
};

int main() {
    CGameCameraManager mgr;
    mgr.vptr = 0;
    mgr.field4.id  = 111;
    mgr.field3c.id = 222;

    CCamera c = mgr.GetRenderCamera(3.5f);

    bool ok = (c.tag == 0xCA) &&
              (c.m == 3.5f) &&
              (c.a == (void*)&mgr.field4) &&
              (c.b == (void*)&mgr.field3c);

    if (ok) printf("PARITY_OK_697f30\n");
    else    printf("FAIL tag=%x m=%f a=%p b=%p\n", c.tag, c.m, c.a, c.b);
    return 0;
}