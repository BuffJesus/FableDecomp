#include <stdio.h>

struct Elem {
    virtual void dtor_slot0(int flag);
    char pad[0x30 - sizeof(void*)];
};

void __fastcall vec_Destroy(Elem* first, Elem* last, void* alloc);

void Elem::dtor_slot0(int) { }

static int g_count = 0;
static int g_lastflag = -99;

struct TestElem : Elem {
    virtual void dtor_slot0(int flag) { g_count++; g_lastflag = flag; }
};

int main()
{
    TestElem arr[4];
    Elem* base = (Elem*)arr;
    // build pointers using 0x30 stride
    Elem* first = base;
    Elem* last  = (Elem*)((char*)base + 4 * 0x30);
    vec_Destroy(first, last, 0);

    if (g_count == 4 && g_lastflag == 0)
        printf("DESTROY_OK count=%d flag=%d\n", g_count, g_lastflag);
    else
        printf("DESTROY_FAIL count=%d flag=%d\n", g_count, g_lastflag);

    // empty range must do nothing
    g_count = 0; g_lastflag = -99;
    vec_Destroy(first, first, 0);
    if (g_count == 0)
        printf("EMPTY_OK\n");
    else
        printf("EMPTY_FAIL\n");

    return (g_count == 0 && g_lastflag == -99) ? 0 : 1;
}