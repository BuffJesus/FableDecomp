#include <cstdio>

static int freed = 0;
static void* freed_ptr = 0;
extern "C" void engine_free(void* p) { freed = 1; freed_ptr = p; }
void* const g_CTCInventoryItem_vtbl = (void*)0x1230ba0;
static int base_called = 0;

struct CTCInventoryItem {
    void* vptr;
    unsigned char _pad_0x4[0x24];
    void* ptr28;
};

static void base_impl(CTCInventoryItem* s) { base_called = 1; }

void CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr28)
        engine_free(self->ptr28);
    self->vptr = g_CTCInventoryItem_vtbl;
    base_impl(self);
}

int main() {
    CTCInventoryItem a;
    a.vptr = 0; a.ptr28 = (void*)0x1234;
    freed = 0; base_called = 0;
    CTCInventoryItem_OnDie(&a);
    if (!freed || freed_ptr != (void*)0x1234) { std::printf("FAIL free\n"); return 1; }
    if (a.vptr != (void*)0x1230ba0) { std::printf("FAIL vtbl\n"); return 1; }
    if (!base_called) { std::printf("FAIL base\n"); return 1; }

    CTCInventoryItem b;
    b.vptr = 0; b.ptr28 = 0;
    freed = 0; base_called = 0;
    CTCInventoryItem_OnDie(&b);
    if (freed) { std::printf("FAIL nofree\n"); return 1; }
    if (b.vptr != (void*)0x1230ba0 || !base_called) { std::printf("FAIL vtbl2\n"); return 1; }

    std::printf("CTCInventoryItem_00453bc4_TEST PASS\n");
    return 0;
}