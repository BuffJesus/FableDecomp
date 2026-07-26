#include <cstdio>

typedef void (__fastcall *VecCtor)(void* self);

void __stdcall vector_constructor_iterator(void* arr, int size, int count, VecCtor ctor)
{
    int m = count - 1;
    if (m >= 0) {
        char* p = (char*)arr;
        int n = m + 1;
        do {
            ctor(p);
            p += size;
            --n;
        } while (n != 0);
    }
}

static int g_calls = 0;
static void* g_last = 0;
static void __fastcall my_ctor(void* self) { g_calls++; g_last = self; }

int main() {
    char buf[40];
    g_calls = 0;
    vector_constructor_iterator(buf, 8, 5, my_ctor);
    if (g_calls == 5 && g_last == buf + 32) {
        std::printf("_global_004038c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}