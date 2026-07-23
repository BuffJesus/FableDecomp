#include <cstdio>

static int g_calls = 0;
static int g_lastFlag = -1;

struct Elem {
    virtual void destroy(int flag) { g_calls++; g_lastFlag = flag; }
    char pad[0x50 - sizeof(void*)];
};

void __fastcall vec__Destroy(Elem* first, Elem* last, int alloc) {
    for (; first != last; first = (Elem*)((char*)first + 0x50)) {
        first->destroy(0);
    }
}

int main() {
    Elem arr[4];
    vec__Destroy(&arr[0], &arr[4], 0);
    if (g_calls == 4 && g_lastFlag == 0) {
        std::printf("vector_CIntelligentPointer_00450ace_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d flag=%d\n", g_calls, g_lastFlag);
    return 1;
}