#include <cstdio>

struct DisplayEngineState {
    int f00;
    int f04;
    int f08;
    int f0c;
    int f10;
    int f14;
    int f18;
    int handle;   // +0x1c
    int f20;
    int f24;
};

void* __fastcall CDisplayEngineInit(DisplayEngineState* self)
{
    self->handle = -1;
    self->f00 = 0;
    self->f04 = 0;
    self->f08 = 0;
    self->f0c = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f18 = 0;
    self->f20 = 0;
    return self;
}

int main()
{
    DisplayEngineState s;
    // Pre-fill every field with sentinel garbage so we can prove the ctor writes.
    s.f00 = 0x11111111;
    s.f04 = 0x22222222;
    s.f08 = 0x33333333;
    s.f0c = 0x44444444;
    s.f10 = 0x55555555;
    s.f14 = 0x66666666;
    s.f18 = 0x77777777;
    s.handle = 0x00000000;
    s.f20 = 0x88888888;
    s.f24 = 0x5AA5A55A;  // must be left untouched

    void* ret = CDisplayEngineInit(&s);

    if (ret != &s) {
        std::printf("FAIL_0x00419270 return\n");
        return 1;
    }
    if (s.handle != -1) {
        std::printf("FAIL_0x00419270 handle\n");
        return 1;
    }
    if (s.f00 || s.f04 || s.f08 || s.f0c || s.f10 || s.f14 || s.f18 || s.f20) {
        std::printf("FAIL_0x00419270 zero\n");
        return 1;
    }
    if (s.f24 != 0x5AA5A55A) {
        std::printf("FAIL_0x00419270 f24-clobbered\n");
        return 1;
    }
    std::printf("OK_0x00419270\n");
    return 0;
}