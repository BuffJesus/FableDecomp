#include <stdio.h>

struct CFlash {
    void *vfptr;
    int   a;
    int   b;
    int   c;
    void Destroy(int flag);
};

template<class T> struct allocator { };

// count of destructor invocations
static int g_destroyed = 0;
static int g_lastFlag  = -1;

void CFlash::Destroy(int flag) {
    g_destroyed++;
    g_lastFlag = flag;
    this->a = 0xDEAD;   // touch memory to prove 'this' is valid
}

void __fastcall _Destroy_range(
        CFlash *first,
        CFlash *last,
        allocator<CFlash> *al);

int main() {
    CFlash arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].vfptr = 0; arr[i].a = i; arr[i].b = i; arr[i].c = i; }

    allocator<CFlash> al;

    _Destroy_range(&arr[0], &arr[5], &al);

    // Expect 5 destructor calls, last flag 0
    if (g_destroyed == 5 && g_lastFlag == 0 && arr[4].a == 0xDEAD) {
        printf("DESTROY_RANGE_OK count=%d flag=%d\n", g_destroyed, g_lastFlag);
        return 0;
    }
    printf("FAIL count=%d flag=%d\n", g_destroyed, g_lastFlag);
    return 1;
}