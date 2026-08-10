#include <cstdio>
struct Inner { char pad[0x128]; float* arr; };
struct CTCDivineWrath { char pad0[0x10]; Inner* f10; char pad1[0x30]; int f44; };
float g_dwConst = 2.0f;
static float model(CTCDivineWrath* self) {
    return self->f10->arr[self->f44] * g_dwConst;
}
int main() {
    float data[4] = {1.0f, 3.0f, 5.0f, 7.0f};
    Inner inr;
    float* pdata = data;
    inr.arr = pdata;
    Inner* pin = &inr;
    CTCDivineWrath w;
    w.f10 = pin;
    CTCDivineWrath* pw = &w;
    w.f44 = 2;
    if (model(pw) != 10.0f) { printf("FAIL2\n"); return 1; }
    w.f44 = 0;
    if (model(pw) != 2.0f) { printf("FAIL0\n"); return 1; }
    w.f44 = 3;
    if (model(pw) != 14.0f) { printf("FAIL3\n"); return 1; }
    printf("OK_00789cc0\n");
    return 0;
}