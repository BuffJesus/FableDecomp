#include <cstdio>
struct CItem;
struct CCategory {
    char pad[8];
    struct Slot { CItem* item; int a, b, c, d; }* slots;
};
CItem* __fastcall GetItemInSlotWithIndex(CCategory* self, int edx_dummy, int index) {
    return index >= 0 ? self->slots[index].item : 0;
}
static CItem* call(CCategory* self, int index) {
    CItem* r;
    __asm {
        mov ecx, self
        mov edx, 0xDEAD
        push index
        call GetItemInSlotWithIndex
        mov r, eax
    }
    return r;
}
int main() {
    CCategory::Slot arr[4];
    for (int i = 0; i < 4; i++) { arr[i].item = (CItem*)(0x1000 + i); arr[i].a = arr[i].b = arr[i].c = arr[i].d = 0; }
    CCategory c; c.slots = arr;
    if (call(&c, -1) != 0) { printf("FAIL neg\n"); return 1; }
    if (call(&c, 0) != (CItem*)0x1000) { printf("FAIL idx0\n"); return 1; }
    if (call(&c, 2) != (CItem*)0x1002) { printf("FAIL idx2\n"); return 1; }
    if (call(&c, 3) != (CItem*)0x1003) { printf("FAIL idx3\n"); return 1; }
    printf("OK_0x005bc9b0\n");
    return 0;
}