#include <cstdio>

struct CFlash {
    char pad[0x14];
    void Dtor(int flag);
};

struct AllocTag {};

static int g_calls = 0;
static int g_argsum = 0;

void CFlash::Dtor(int flag) {
    g_calls++;
    g_argsum += flag;
    pad[0] = (char)0xAB; // touch memory
}

void __fastcall Destroy_range_flash(CFlash* first, CFlash* last,
                                    AllocTag* al)
{
    for (; first != last; ++first)
        first->Dtor(0);
}

int main() {
    CFlash arr[5];
    AllocTag al;
    Destroy_range_flash(arr, arr + 5, &al);
    if (g_calls == 5 && g_argsum == 0) {
        printf("DESTROY_RANGE_OK\n");
        return 0;
    }
    printf("FAIL calls=%d sum=%d\n", g_calls, g_argsum);
    return 1;
}