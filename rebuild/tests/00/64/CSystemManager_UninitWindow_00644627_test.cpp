#include <stdio.h>

struct Temp { int slot; };

struct CSystemManager {
    char pad0[0x18];
    unsigned char byte18;
    int   field1c;
    int   field20;
    void UninitWindow();
};

static int g_called = 0;
static int g_edx = 0, g_a0 = 0, g_a1 = 0;

void __fastcall Temp_helper(Temp* self, int edx, int a0, int a1)
{
    (void)self;
    g_called = 1;
    g_edx = edx; g_a0 = a0; g_a1 = a1;
}

int main()
{
    CSystemManager s;
    s.byte18 = 0;
    s.field1c = 0x1111;
    s.field20 = 0x2222;
    g_called = 0;
    s.UninitWindow();
    if (g_called && g_edx == 0x1111 && g_a0 == 0x2222 && g_a1 == 0x41be70) {
        CSystemManager s2;
        s2.byte18 = 1;
        s2.field1c = 5; s2.field20 = 6;
        g_called = 0;
        s2.UninitWindow();
        if (g_called == 0) {
            printf("UNINITWINDOW_OK\n");
            return 0;
        }
    }
    printf("FAIL\n");
    return 1;
}