#include <cstdio>

static long g_stdcall_arg1 = -1;
static long g_stdcall_arg2 = -1;
static void* g_thiscall_self = 0;
static long g_thiscall_arg = -1;

void __stdcall Sub_5b43a2(long item, long member4) {
    g_stdcall_arg1 = member4;
    g_stdcall_arg2 = item;
}

struct CTCInventoryClothing {
    void* vtbl;
    long  member4;
    void Sub_5bdfb1(long item);
    void AddClothing(long item);
};

void CTCInventoryClothing::Sub_5bdfb1(long item) {
    g_thiscall_self = this;
    g_thiscall_arg = item;
}

void CTCInventoryClothing::AddClothing(long item)
{
    Sub_5b43a2(item, this->member4);
    this->Sub_5bdfb1(item);
}

int main() {
    CTCInventoryClothing inv;
    inv.vtbl = 0;
    inv.member4 = 0x1234;
    inv.AddClothing(0x42);
    if (g_stdcall_arg1 == 0x1234 && g_stdcall_arg2 == 0x42 &&
        g_thiscall_self == &inv && g_thiscall_arg == 0x42) {
        printf("PARITY_OK\n");
    } else {
        printf("PARITY_FAIL\n");
    }
    return 0;
}