struct CMemoryAllocatorVariableSize_Overlay {
    CScriptDataBase* header;   // 0x00
    std::uint32_t count;       // 0x04
    std::byte less_storage[sizeof(std::less<CCharString>)]; // 0x08
};

static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, header) == 0x00);
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, count) == 0x04);
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, less_storage) == 0x08);

struct CScriptDataBase_Overlay {
    std::byte _pad0[0x04];
    CScriptDataBase* link_04;  // 0x04
    CScriptDataBase* link_08;  // 0x08
    CScriptDataBase* link_0C;  // 0x0C
    std::byte key_storage[sizeof(CCharString)]; // 0x10
};

static_assert(offsetof(CScriptDataBase_Overlay, link_04) == 0x04);
static_assert(offsetof(CScriptDataBase_Overlay, link_08) == 0x08);
static_assert(offsetof(CScriptDataBase_Overlay, link_0C) == 0x0C);
static_assert(offsetof(CScriptDataBase_Overlay, key_storage) == 0x10);

extern void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    CScriptDataBase* node,
    void* root_link);

void __thiscall CMemoryAllocatorVariableSize_InsertNode(
    CMemoryAllocatorVariableSize* self,
    CScriptDataBase** out_node,
    int param_3,
    CScriptDataBase* parent,
    CCharString* key,
    int param_6)
{
    auto* const alloc = reinterpret_cast<CMemoryAllocatorVariableSize_Overlay*>(self);

    bool less_result;
    CScriptDataBase* new_node;
    CScriptDataBase* header_ptr;

    if (parent == alloc->header) {
insert_left:
        new_node = NScript::CDanyalTestScriptsData::Alloc();
        reinterpret_cast<CScriptDataBase_Overlay*>(parent)->link_08 = new_node;

        header_ptr = alloc->header;
        if (parent != header_ptr) {
            if (parent == reinterpret_cast<CScriptDataBase_Overlay*>(header_ptr)->link_08) {
                reinterpret_cast<CScriptDataBase_Overlay*>(header_ptr)->link_08 = new_node;
            }
            goto finish_insert;
        }

        reinterpret_cast<CScriptDataBase_Overlay*>(header_ptr)->link_04 = new_node;
        header_ptr = alloc->header;
    } else {
        if (param_6 == 0) {
            if (param_3 == 0) {
                less_result =
                    (*reinterpret_cast<std::less<CCharString>*>(alloc->less_storage))(
                        *key,
                        *reinterpret_cast<CCharString*>(
                            reinterpret_cast<CScriptDataBase_Overlay*>(parent)->key_storage));
                if (!less_result) {
                    goto insert_right;
                }
            }
            goto insert_left;
        }

insert_right:
        new_node = NScript::CDanyalTestScriptsData::Alloc();
        reinterpret_cast<CScriptDataBase_Overlay*>(parent)->link_0C = new_node;

        header_ptr = alloc->header;
        if (parent != reinterpret_cast<CScriptDataBase_Overlay*>(header_ptr)->link_0C) {
            goto finish_insert;
        }
    }

    reinterpret_cast<CScriptDataBase_Overlay*>(header_ptr)->link_0C = new_node;

finish_insert:
    reinterpret_cast<CScriptDataBase_Overlay*>(new_node)->link_08 = nullptr;
    reinterpret_cast<CScriptDataBase_Overlay*>(new_node)->link_0C = nullptr;
    reinterpret_cast<CScriptDataBase_Overlay*>(new_node)->link_04 = parent;

    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::byte*>(alloc->header) + 0x04);

    ++alloc->count;
    *out_node = new_node;
}