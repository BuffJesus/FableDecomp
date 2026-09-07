#include "engine/CCameraCollisionDef.h"
#include <cstdio>
struct CDefClassBase;


static int g_called = 0;
extern "C" void base_copy_called() { g_called = 1; }

void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src);

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src);

void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src) {
    base_copy_called();
}

int main() {
    CCameraCollisionDef a, b;
    b.CameraCollisionMesh = 0xDEADBEEF;
    a.CameraCollisionMesh = 0;
    CCameraCollisionDef_Copy(&a, 0, (const CDefClassBase*)&b);
    if (a.CameraCollisionMesh == 0xDEADBEEF && g_called == 1) {
        printf("004e187e_TEST PASS\n");
    } else {
        printf("FAIL a=%08lx called=%d\n", a.CameraCollisionMesh, g_called);
    }
    return 0;
}