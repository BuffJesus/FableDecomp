#include <cstddef>
#include <cstdint>

struct VectorMapEntry_long_CThingPtr_Overlay {
    std::int32_t key_;
    void* object_;
};

static_assert(offsetof(VectorMapEntry_long_CThingPtr_Overlay, key_) == 0x0);
static_assert(offsetof(VectorMapEntry_long_CThingPtr_Overlay, object_) == 0x4);

struct CVectorMap_long_CIntelligentPointer_CThing___Overlay {
    std::byte unknown_0[0x4];
    VectorMapEntry_long_CThingPtr_Overlay* end_;
};

static_assert(offsetof(CVectorMap_long_CIntelligentPointer_CThing___Overlay, end_) == 0x4);

struct VectorMapLookupValue_This_Overlay {
    std::byte unknown_0000[0x7A8];
    CVectorMap_long_CIntelligentPointer_CThing___Overlay map_;
};

static_assert(offsetof(VectorMapLookupValue_This_Overlay, map_) == 0x7A8);

using LowerBoundFn = VectorMapEntry_long_CThingPtr_Overlay*(__thiscall*)(
    CVectorMap_long_CIntelligentPointer_CThing___Overlay*,
    const std::int32_t*);

extern LowerBoundFn CVectorMap_long_CIntelligentPointer_CThing___LowerBound;

std::uint32_t __thiscall VectorMap_LookupValue(void* self, void* param_2, std::int32_t key)
{
    auto* const this_ = static_cast<VectorMapLookupValue_This_Overlay*>(self);

    VectorMapEntry_long_CThingPtr_Overlay* it =
        CVectorMap_long_CIntelligentPointer_CThing___LowerBound(&this_->map_, &key);

    if ((it == this_->map_.end_) || (key < it->key_)) {
        it = this_->map_.end_;
    }

    if (it != this_->map_.end_) {
        void* const raw_object = it->object_;
        void** const vftable = *reinterpret_cast<void***>(raw_object);
        using IndirectCall = std::uint32_t(__thiscall*)(void*, void*);
        return reinterpret_cast<IndirectCall>(vftable[7])(raw_object, param_2);
    }

    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(it) & 0xFFFFFF00u);
}