#include <cstdio>
struct CCategory { int dummy; };
struct CTCInventoryBase {
    char pad[0xc0];
    CCategory* categories;
};
CCategory* __fastcall CTCInventoryBase_GetCategoryWithIndex(CTCInventoryBase* self, void* /*edx*/, int index) {
    return self->categories + index;
}
int main() {
    CCategory arr[8];
    CTCInventoryBase obj;
    obj.categories = arr;
    if (CTCInventoryBase_GetCategoryWithIndex(&obj, 0, 0) != &arr[0]) { std::printf("FAIL idx0\n"); return 1; }
    if (CTCInventoryBase_GetCategoryWithIndex(&obj, 0, 3) != &arr[3]) { std::printf("FAIL idx3\n"); return 1; }
    if (CTCInventoryBase_GetCategoryWithIndex(&obj, 0, 7) != &arr[7]) { std::printf("FAIL idx7\n"); return 1; }
    std::printf("CTCInventoryBase_00407360_TEST PASS\n");
    return 0;
}