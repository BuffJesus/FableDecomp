#include <cstddef>
#include <cstdint>

extern const std::byte DAT_012a9a24;

struct EnumerateQueryInterface_CollectionVTable {
    std::byte pad_00[0x18];
    int(__thiscall* vf0x18)(void* self);
    void*(__thiscall* vf0x1C)(void* self, int index);
};
static_assert(offsetof(EnumerateQueryInterface_CollectionVTable, vf0x18) == 0x18);
static_assert(offsetof(EnumerateQueryInterface_CollectionVTable, vf0x1C) == 0x1C);

struct EnumerateQueryInterface_ItemIfaceVTable {
    std::byte pad_00[0x18];
    int(__stdcall* vf0x18)(void* self, void** out_object);
    std::byte pad_1C[0x08];
    int(__stdcall* vf0x24)(void* self, int* out_type);
};
static_assert(offsetof(EnumerateQueryInterface_ItemIfaceVTable, vf0x18) == 0x18);
static_assert(offsetof(EnumerateQueryInterface_ItemIfaceVTable, vf0x24) == 0x24);

struct EnumerateQueryInterface_ComLikeVTable {
    int(__stdcall* vf0x00)(void* self, const void* iid, void** out_object);
    std::byte pad_04[0x04];
    void(__stdcall* vf0x08)(void* self);
    std::byte pad_0C[0x14];
    int(__stdcall* vf0x20)(void* self);
};
static_assert(offsetof(EnumerateQueryInterface_ComLikeVTable, vf0x00) == 0x00);
static_assert(offsetof(EnumerateQueryInterface_ComLikeVTable, vf0x08) == 0x08);
static_assert(offsetof(EnumerateQueryInterface_ComLikeVTable, vf0x20) == 0x20);

struct EnumerateQueryInterface_ThisOverlay {
    std::byte pad_00[0x70];
    void* collection;
};
static_assert(offsetof(EnumerateQueryInterface_ThisOverlay, collection) == 0x70);

std::uint32_t __stdcall Enumerate_QueryInterface_012a9a24(void* param_1)
{
    auto* const self = reinterpret_cast<EnumerateQueryInterface_ThisOverlay*>(
        reinterpret_cast<std::byte*>(param_1) - 0x70);

    void* const collection = self->collection;
    auto* const collection_vtbl =
        *reinterpret_cast<EnumerateQueryInterface_CollectionVTable* const*>(collection);

    const int count = collection_vtbl->vf0x18(collection);
    int matched_count = 0;
    int index = 0;

    if (0 < count) {
        do {
            void* const item = collection_vtbl->vf0x1C(collection, index);
            void* const item_iface =
                reinterpret_cast<void*>(reinterpret_cast<std::byte*>(item) + 0x0C);
            auto* const item_iface_vtbl =
                *reinterpret_cast<EnumerateQueryInterface_ItemIfaceVTable* const*>(item_iface);

            int item_type = 0;
            const int get_type_result = item_iface_vtbl->vf0x24(item_iface, &item_type);
            if (get_type_result < 0) {
                return static_cast<std::uint32_t>(get_type_result);
            }

            if (item_type == 1) {
                void* queried_object = nullptr;
                const int get_interface_result =
                    item_iface_vtbl->vf0x18(item_iface, &queried_object);
                if (-1 < get_interface_result) {
                    ++matched_count;

                    auto* const queried_object_vtbl =
                        *reinterpret_cast<EnumerateQueryInterface_ComLikeVTable* const*>(
                            queried_object);

                    void* queried_interface = nullptr;
                    const int query_result = queried_object_vtbl->vf0x00(
                        queried_object,
                        &DAT_012a9a24,
                        &queried_interface);

                    queried_object_vtbl->vf0x08(queried_object);

                    if (query_result < 0) {
                        return 0;
                    }

                    auto* const queried_interface_vtbl =
                        *reinterpret_cast<EnumerateQueryInterface_ComLikeVTable* const*>(
                            queried_interface);

                    const int method_result =
                        queried_interface_vtbl->vf0x20(queried_interface);
                    queried_interface_vtbl->vf0x08(queried_interface);

                    if (method_result != 1) {
                        return 0;
                    }
                }
            }

            ++index;
        } while (index < count);
    }

    return static_cast<std::uint32_t>(matched_count != 0);
}