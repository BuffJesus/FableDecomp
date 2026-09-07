struct BSTKeyStringOverlay
{
    char* text;
};
static_assert(offsetof(BSTKeyStringOverlay, text) == 0x0);

struct BSTKeyWrapperOverlay
{
    BSTKeyStringOverlay* key_ptr;
};
static_assert(offsetof(BSTKeyWrapperOverlay, key_ptr) == 0x0);

struct BSTNodeOverlay
{
    std::byte pad_00[0x08];
    BSTNodeOverlay* child_at_08;
    BSTNodeOverlay* child_at_0C;
    BSTKeyStringOverlay* key_at_10;
};
static_assert(offsetof(BSTNodeOverlay, child_at_08) == 0x08);
static_assert(offsetof(BSTNodeOverlay, child_at_0C) == 0x0C);
static_assert(offsetof(BSTNodeOverlay, key_at_10) == 0x10);

struct BSTStorageOverlay
{
    std::byte pad_00[0x04];
    BSTNodeOverlay* root_at_04;
};
static_assert(offsetof(BSTStorageOverlay, root_at_04) == 0x04);

struct BSTStorageWrapperOverlay
{
    BSTStorageOverlay* storage_ptr;
};
static_assert(offsetof(BSTStorageWrapperOverlay, storage_ptr) == 0x0);

int __cdecl CBasicString<char>::Compare(const char* lhs, const char* rhs);
void __cdecl BinaryTree_InsertDigScriptNode(
    void* insertContext,
    int zeroA,
    BSTNodeOverlay* parent,
    BSTKeyWrapperOverlay* key,
    int zeroB);

void* __thiscall BinarySearchTree_FindOrInsert(
    BSTStorageWrapperOverlay* self,
    void* insertContext,
    BSTKeyWrapperOverlay* key)
{
    BSTStorageOverlay* const storage = self->storage_ptr;
    BSTNodeOverlay* parent = reinterpret_cast<BSTNodeOverlay*>(storage);

    if (storage->root_at_04 != nullptr)
    {
        BSTKeyStringOverlay* const searchKey = key->key_ptr;
        BSTNodeOverlay* current = storage->root_at_04;

        do
        {
            parent = current;
            BSTKeyStringOverlay* const nodeKey = parent->key_at_10;

            if ((nodeKey == searchKey) ||
                ((searchKey != nullptr) &&
                 ((nodeKey == nullptr) ||
                  (CBasicString<char>::Compare(searchKey->text, nodeKey->text) >= 0))))
            {
                current = parent->child_at_0C;
            }
            else
            {
                current = parent->child_at_08;
            }
        } while (current != nullptr);
    }

    BinaryTree_InsertDigScriptNode(insertContext, 0, parent, key, 0);
    return insertContext;
}