#include <cstdio>
struct CDefClassBase;

struct CCameraCollisionDef {
    char pad[0x28];
    unsigned long field28;
};

static int g_called = 0;
extern "C" void base_copy_called() { g_called = 1; }

void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src);

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src);

void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src) {
    base_copy_called();
}

int main() {
    CCameraCollisionDef a, b;
    b.field28 = 0xDEADBEEF;
    a.field28 = 0;
    CCameraCollisionDef_Copy(&a, 0, (const CDefClassBase*)&b);
    if (a.field28 == 0xDEADBEEF && g_called == 1) {
        printf("004e187e_TEST PASS\n");
    } else {
        printf("FAIL a=%08lx called=%d\n", a.field28, g_called);
    }
    return 0;
}