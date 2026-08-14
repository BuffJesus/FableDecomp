#include <cstdio>

static int g_deleted = 0;
static int g_subdtor = 0;
static void* g_sub_self = 0;
static void* g_del_self = 0;

// mirrors Sub_dtor(self+4)
void sub_dtor(void* subself) { g_subdtor++; g_sub_self = subself; }
// mirrors operator delete(self)
void op_delete(void* p) { g_deleted++; g_del_self = p; }

void* vdd(void* self, unsigned int flags)
{
    sub_dtor(static_cast<char*>(self) + 4);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    char buf[64];
    void* obj = buf;

    // flags bit0 clear: dtor runs on self+4, no delete
    g_subdtor = 0; g_deleted = 0; g_sub_self = 0; g_del_self = 0;
    void* r = vdd(obj, 0);
    if (r != obj || g_subdtor != 1 || g_deleted != 0) { printf("FAIL1\n"); return 1; }
    if (g_sub_self != static_cast<char*>(obj) + 4) { printf("FAIL1b\n"); return 1; }

    // flags bit0 set: dtor runs AND delete(self)
    g_subdtor = 0; g_deleted = 0; g_sub_self = 0; g_del_self = 0;
    r = vdd(obj, 1);
    if (r != obj || g_subdtor != 1 || g_deleted != 1) { printf("FAIL2\n"); return 1; }
    if (g_del_self != obj) { printf("FAIL2b\n"); return 1; }

    // even flags (bit0 clear) do not delete
    g_subdtor = 0; g_deleted = 0;
    vdd(obj, 2);
    if (g_deleted != 0 || g_subdtor != 1) { printf("FAIL3\n"); return 1; }

    // odd flags >1 (bit0 set) delete
    g_subdtor = 0; g_deleted = 0;
    vdd(obj, 3);
    if (g_deleted != 1) { printf("FAIL4\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}