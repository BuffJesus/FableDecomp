#include <stdio.h>

struct RefObj {
    void** vtbl;
    int    refcnt;
    char   pad[0x54];
    float  speed;
};

struct CTCWeapon {
    char    pad[0x70];
    RefObj* obj;
};

static int g_destroyed = 0;

struct SmartPtr {
    RefObj* p;
    SmartPtr(RefObj* q) : p(q) { if (p) p->refcnt++; }
    ~SmartPtr() {
        if (p && --p->refcnt == 0) {
            ((void (__fastcall*)(RefObj*))p->vtbl[1])(p);
        }
    }
};

float GetAnimationSpeedValue(CTCWeapon* self) {
    SmartPtr sp(self->obj);
    return sp.p->speed;
}

void __fastcall fake_dtor(RefObj* r) { g_destroyed = 1; }

int main() {
    // vtbl slot 1 = destructor
    static void* vt[2] = { 0, (void*)&fake_dtor };
    RefObj obj;
    obj.vtbl = vt;
    obj.refcnt = 5;      // held elsewhere; scoped add/release nets zero, not destroyed
    obj.speed = 2.5f;

    CTCWeapon w;
    w.obj = &obj;

    float v = GetAnimationSpeedValue(&w);
    // refcount restored, object not destroyed
    if (v == 2.5f && obj.refcnt == 5 && g_destroyed == 0) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL v=%f rc=%d d=%d\n", v, obj.refcnt, g_destroyed);
    }
    return 0;
}