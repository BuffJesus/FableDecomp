#include <cstdio>

struct CArchive;
static void* g_base_self; static void* g_base_ar;
static void* g_a_ar; static void* g_a_p;
static void* g_b_ar; static void* g_b_p;

struct CArchive {
    void TransferA(void* p){ g_a_ar=this; g_a_p=p; }
    void TransferB(void* p){ g_b_ar=this; g_b_p=p; }
};

struct CBase {
    char pad[0x70];
    char field70[0xE];
    char field7e[0x10];
    void BaseTransfer(CArchive* ar){ g_base_self=this; g_base_ar=ar; }
};

void __fastcall CThingHolySiteDef_Transfer(CBase* self, void* edx, CArchive* ar)
{
    self->BaseTransfer(ar);
    ar->TransferA(&self->field70);
    ar->TransferB(self->field7e);
}

int main()
{
    CBase b; CArchive a;
    CThingHolySiteDef_Transfer(&b, 0, &a);
    bool ok = (g_base_self==&b) && (g_base_ar==&a)
        && (g_a_ar==&a) && (g_a_p==(void*)&b.field70)
        && (g_b_ar==&a) && (g_b_p==(void*)&b.field7e);
    if(ok){ std::printf("CThingHolySiteDef_00454dcb_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}