// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x007d9ef0
struct CTCInventoryItem;

// cdecl helper @ 0xbfea14  (add esp,4 after call => __cdecl, one ptr arg)
void __cdecl SomeHelper(void* p);

// base OnDie member @ 0x5eda50 (this-call, void)
void __fastcall CTCInventoryItem_BaseOnDie(CTCInventoryItem* self);

struct CTCInventoryItem
{
    char pad[0x110];
    void* m_ptr; // at +0x110
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->m_ptr;
    if (p != 0)
        SomeHelper(p);
    CTCInventoryItem_BaseOnDie(self);
}