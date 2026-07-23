struct CInventoryItemDef { char pad[4]; };
extern void __fastcall CInventoryItemDef_Copy_impl(CInventoryItemDef* self, void* src);
void __fastcall CInventoryItemDef_Copy(CInventoryItemDef* self, void* src) {
    CInventoryItemDef_Copy_impl(self, src);
}