#include <cstdio>

struct CTCInventoryBase {
    char pad20[0x20];
    void* member20;
    void* member24;
};

static int g_log = 0;

static void* sub_5D97B0(long value) { g_log += value; return (void*)(value + 1); }
static void* sub_5C08C0(void* self, void* edxArg, void* stackArg) {
    g_log += (int)(long long)stackArg;
    return (void*)((char*)self + (long)edxArg);
}
static int g_final = 0;
static void sub_5BF1D7(void* self, void* out, long a, bool b) {
    g_final = (int)(long long)self + (int)a + (b ? 100 : 0);
}

static void GetItemDescription(CTCInventoryBase* self, void* out, long itemId, bool flag) {
    void* t = sub_5D97B0(itemId);
    void* r = sub_5C08C0(self->member20, self->member24, t);
    sub_5BF1D7(r, out, itemId, flag);
}

int main() {
    CTCInventoryBase inv;
    inv.member20 = (void*)10;
    inv.member24 = (void*)5;
    GetItemDescription(&inv, (void*)0, 7, true);
    // t = 8; r = 10+5 = 15; final = 15 + 7 + 100 = 122
    if (g_final == 122 && g_log == (7 + 8)) {
        printf("BEHAVIOR_OK\n");
    } else {
        printf("BEHAVIOR_FAIL %d %d\n", g_final, g_log);
    }
    return 0;
}