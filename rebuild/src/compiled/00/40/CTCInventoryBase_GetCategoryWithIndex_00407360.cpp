struct CCategory { int dummy; };
struct CTCInventoryBase {
    char pad[0xc0];
    CCategory* categories;
};
CCategory* __fastcall CTCInventoryBase_GetCategoryWithIndex(CTCInventoryBase* self, void* /*edx*/, int index) {
    return self->categories + index;
}