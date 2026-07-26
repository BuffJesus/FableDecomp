#include <cstdio>
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x8];
    void* ptr8;
    unsigned char _pad_0xc[0x4];
    void* ptr10;
    unsigned char _pad_0x14[0xc];
    char sub20;
};
static int g_freed = 0;
static int g_base = 0;
static void* g_freed_ptr = 0;
extern "C" void __cdecl eng_free(void* p){ g_freed++; g_freed_ptr = p; }
void __fastcall CTCInventoryItem_base_OnDie(CTCInventoryItem* self){ (void)self; g_base++; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr8;
    if (p)
        eng_free(p);
    CTCInventoryItem_base_OnDie(self);
}

int main(){
    CTCInventoryItem a;
    int dummy = 0;
    a.ptr8 = &dummy;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_freed_ptr != &dummy || g_base != 1){ std::printf("FAIL non-null path\n"); return 1; }
    g_freed = 0; g_base = 0;
    a.ptr8 = 0;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 0 || g_base != 1){ std::printf("FAIL null path\n"); return 1; }
    std::printf("CTCInventoryItem_00455e59_TEST PASS\n");
    return 0;
}