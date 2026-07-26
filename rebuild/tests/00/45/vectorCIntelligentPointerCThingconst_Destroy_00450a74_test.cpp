#include <cstdio>

static int g_calls = 0;
static int g_arg_seen = 999;

struct Elem {
    virtual void* scalar_dtor(int flag) {
        g_calls++;
        g_arg_seen = flag;
        return this;
    }
    char pad[0x4c - 4];
};

void __fastcall vector_Destroy(Elem* first, Elem* last, void* al)
{
    while (first != last) {
        first->scalar_dtor(0);
        first = (Elem*)((char*)first + 0x4c);
    }
}

int main() {
    Elem arr[3];
    vector_Destroy(&arr[0], &arr[3], (void*)0);
    if (g_calls == 3 && g_arg_seen == 0) {
        std::printf("vector_00450a74_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d arg=%d\n", g_calls, g_arg_seen);
    return 1;
}