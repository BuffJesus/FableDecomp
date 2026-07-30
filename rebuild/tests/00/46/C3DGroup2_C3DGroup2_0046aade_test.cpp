#include <cstdio>

struct T3DGroup2 {
    int f0;
    int f4;
    int f8;
    int fc;
};

T3DGroup2* __fastcall C3DGroup2(T3DGroup2* pThis) {
    pThis->f0 = 0;
    pThis->f4 = -1;
    pThis->f8 = -1;
    pThis->fc = -1;
    return pThis;
}

int main() {
    T3DGroup2 obj;
    obj.f0 = 0x11111111;
    obj.f4 = 0x22222222;
    obj.f8 = 0x33333333;
    obj.fc = 0x44444444;
    T3DGroup2* ret;
    T3DGroup2* pobj = &obj;
    // Invoke via __asm shim: this in ecx (__fastcall/__fastcall).
    __asm {
        mov ecx, pobj
        call C3DGroup2
        mov ret, eax
    }
    if (ret != pobj) { printf("FAIL: return not this\n"); return 1; }
    if (obj.f0 != 0) { printf("FAIL: f0=%d\n", obj.f0); return 1; }
    if (obj.f4 != -1) { printf("FAIL: f4=%d\n", obj.f4); return 1; }
    if (obj.f8 != -1) { printf("FAIL: f8=%d\n", obj.f8); return 1; }
    if (obj.fc != -1) { printf("FAIL: fc=%d\n", obj.fc); return 1; }
    printf("OK_0x0046aade\n");
    return 0;
}