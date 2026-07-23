extern void* g_vtbl_CTCInventoryItem;
extern "C" void __fastcall SomeCleanup(void* self);
extern "C" void __fastcall SubMemberDtor(void* sub);
extern "C" void __cdecl FreeMem(void* p);
extern "C" void __fastcall BaseOnDie(void* self);

struct CTCInventoryItem {
    void* vtbl;      // +0x00
    char pad0[0x0C]; // +0x04..0x0F
    void* ptr10;     // +0x10
    char pad1[0x0C]; // +0x14..0x1F
    char sub20[0x04];// +0x20
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    self->vtbl = &g_vtbl_CTCInventoryItem;
    SomeCleanup(self);
    SubMemberDtor(&self->sub20);
    if (self->ptr10) {
        FreeMem(self->ptr10);
    }
    BaseOnDie(self);
}