#include <cstdio>

static int g_base_called = 0;
static int g_push_called = 0;
static int g_push_this   = 0;

struct CMember {
    char pad[0x10];
    unsigned char flag;                    // +0x10
    void DoPush(int a, int b);             // __fastcall
};

struct CModeChargeForcePush {
    void*    base;        // +0x00
    CMember* member;      // +0x04
};

extern void __fastcall CMode_Base_OnActivate(void* self);

void __fastcall CMode_Base_OnActivate(void* self) { (void)self; g_base_called++; }

void CMember::DoPush(int a, int b) {
    (void)a; (void)b;
    g_push_called++;
    g_push_this += (int)(this->flag);   // prove 'this' is the member
}

void __fastcall CModeChargeForcePush_OnActivate(CModeChargeForcePush* self);

int main()
{
    CMember m;
    CModeChargeForcePush obj;
    obj.member = &m;

    // flag == 1 -> base + push, and push's this is &m (flag==1)
    g_base_called = 0; g_push_called = 0; g_push_this = 0;
    m.flag = 1;
    CModeChargeForcePush_OnActivate(&obj);
    bool ok1 = (g_base_called == 1) && (g_push_called == 1) && (g_push_this == 1);

    // flag == 0 -> only base
    g_base_called = 0; g_push_called = 0; g_push_this = 0;
    m.flag = 0;
    CModeChargeForcePush_OnActivate(&obj);
    bool ok2 = (g_base_called == 1) && (g_push_called == 0);

    // flag == 2 -> only base
    g_base_called = 0; g_push_called = 0; g_push_this = 0;
    m.flag = 2;
    CModeChargeForcePush_OnActivate(&obj);
    bool ok3 = (g_base_called == 1) && (g_push_called == 0);

    if (ok1 && ok2 && ok3) { printf("OFP_OK\n"); return 0; }
    printf("OFP_FAIL\n"); return 1;
}