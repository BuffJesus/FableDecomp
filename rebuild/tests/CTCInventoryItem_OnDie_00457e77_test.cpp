#include <cstdio>
struct CTCInventoryItem { void* vptr; char _pad4[0x44]; void* ptr48; };
static int g_freed = 0;
static int g_based = 0;
extern "C" void freeThing(void* p){ (void)p; g_freed++; }
void __fastcall CTCInventoryItem_base_OnDie(CTCInventoryItem* self){ (void)self; g_based++; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr48;
    if (p) {
        freeThing(p);
    }
    CTCInventoryItem_base_OnDie(self);
}

int main(){
    CTCInventoryItem a; a.ptr48 = (void*)1;
    CTCInventoryItem_OnDie(&a);
    if (g_freed != 1 || g_based != 1) { std::printf("FAIL nonnull path\n"); return 1; }
    CTCInventoryItem b; b.ptr48 = 0; g_freed=0; g_based=0;
    CTCInventoryItem_OnDie(&b);
    if (g_freed != 0 || g_based != 1) { std::printf("FAIL null path\n"); return 1; }
    std::printf("CTCInventoryItem_00457e77_TEST PASS\n");
    return 0;
}