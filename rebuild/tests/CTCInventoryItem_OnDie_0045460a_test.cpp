#include <cstdio>
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x10];
    void* ptr10;
    unsigned char _pad_0x14[0xc];
    char sub20;
    unsigned char _pad_0x21[0xf];
    void* ptr30;
};
static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;
void engine_free(void* p){ g_freed++; g_freed_ptr = p; }
void CTCInventoryItem_Base_OnDie(CTCInventoryItem* s){ g_base_called++; }
const void* CTCInventoryItem_vftable_storage = 0;
const void* const CTCInventoryItem_vftable = (const void*)0x1230ba0;
void CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr30)
        engine_free(self->ptr30);
    *(const void**)self = &CTCInventoryItem_vftable;
    CTCInventoryItem_Base_OnDie(self);
}
int main(){
    CTCInventoryItem a;
    int dummy = 0;
    a.ptr30 = &dummy;
    *(void**)&a = 0;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_freed_ptr != &dummy) { std::printf("FAIL free\n"); return 1; }
    if (g_base_called != 1) { std::printf("FAIL base\n"); return 1; }
    if (*(const void**)&a != &CTCInventoryItem_vftable) { std::printf("FAIL vt\n"); return 1; }
    CTCInventoryItem b;
    b.ptr30 = 0;
    *(void**)&b = 0;
    g_freed = 0; g_base_called = 0;
    CTCInventoryItem_OnDie(&b);
    if (g_freed != 0) { std::printf("FAIL nofree\n"); return 1; }
    if (g_base_called != 1) { std::printf("FAIL base2\n"); return 1; }
    std::printf("CTCInventoryItem_0045460a_TEST PASS\n");
    return 0;
}