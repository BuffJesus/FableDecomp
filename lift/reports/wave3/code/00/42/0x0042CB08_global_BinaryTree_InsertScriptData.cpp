struct ScriptDataBaseTreeNodeOverlay
{
    std::byte _pad0[0x04];
    CScriptDataBase* parent; // 0x04
    CScriptDataBase* left;   // 0x08
    CScriptDataBase* right;  // 0x0C
    std::uint32_t key;       // 0x10
};

static_assert(offsetof(ScriptDataBaseTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(ScriptDataBaseTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(ScriptDataBaseTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(ScriptDataBaseTreeNodeOverlay, key) == 0x10);

struct ScriptDataBaseTreeOverlay
{
    ScriptDataBaseTreeNodeOverlay* header; // 0x00
    int count;                             // 0x04
};

static_assert(offsetof(ScriptDataBaseTreeOverlay, header) == 0x00);
static_assert(offsetof(ScriptDataBaseTreeOverlay, count) == 0x04);

void __thiscall ::BinaryTree_InsertScriptData(
    ScriptDataBaseTreeOverlay* tree,
    CScriptDataBase** outInserted,
    int hasLowerBound,
    ScriptDataBaseTreeNodeOverlay* parent,
    const std::uint32_t* key,
    int insertRight
)
{
    CScriptDataBase* insertedNode;
    ScriptDataBaseTreeNodeOverlay* header;

    if ((parent == tree->header) ||
        ((insertRight == 0) && ((hasLowerBound != 0) || (*key < parent->key))))
    {
        insertedNode = NScript::CDanyalTestScriptsData::Alloc();
        reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(parent)->left = insertedNode;

        header = tree->header;
        if (parent != header)
        {
            if (parent == reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(header->left))
            {
                header->left = insertedNode;
            }
            goto finish_link;
        }

        header->parent = insertedNode;
        header = tree->header;
    }
    else
    {
        insertedNode = NScript::CDanyalTestScriptsData::Alloc();
        reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(parent)->right = insertedNode;

        header = tree->header;
        if (parent != reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(header->right))
        {
            goto finish_link;
        }
    }

    header->right = insertedNode;

finish_link:
    reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(insertedNode)->left = nullptr;
    reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(insertedNode)->right = nullptr;
    reinterpret_cast<ScriptDataBaseTreeNodeOverlay*>(insertedNode)->parent =
        reinterpret_cast<CScriptDataBase*>(parent);

    CMemoryAllocatorVariableSize__BalanceTree(
        insertedNode,
        reinterpret_cast<int>(tree->header) + 4
    );

    tree->count = tree->count + 1;
    *outInserted = insertedNode;
}