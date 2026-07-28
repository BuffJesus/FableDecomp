#include <cstddef>
#include <cstdint>
#include <functional>

struct CPersistTraits_NodeKeyOverlay {
    std::byte pad_00[0x10];
    CCharString key;
};
static_assert(offsetof(CPersistTraits_NodeKeyOverlay, key) == 0x10);

struct CPersistTraits_NodeValueOverlay {
    std::byte pad_00[0x14];
    CStringParser value;
};
static_assert(offsetof(CPersistTraits_NodeValueOverlay, value) == 0x14);

using OptimisedPrimitivePtr =
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>;

using MorphEntryPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

struct CPersistTraits_TransferInResult {
    std::uint32_t low;
    std::uint32_t high;
};
static_assert(sizeof(CPersistTraits_TransferInResult) == 8);

// std::pair<CCharString, CTCBase::CRegisteredTCInfo>::pair(...)
extern void __thiscall Construct_RegisteredInfoPair(
    void* destination,
    CCharString* first,
    CTCBase::CRegisteredTCInfo* second);

// std::less<CCharString>::operator()(...)
extern bool __thiscall Less_CCharString(
    std::less<CCharString>* self,
    CCharString* lhs,
    CCharString* rhs);

// std::_Dest_val<std::allocator<CCountedPointer<...>>, CCountedPointer<...>>(...)
extern void __thiscall STL_DestVal_OptimisedPrimitive(
    void* storage,
    OptimisedPrimitivePtr* value);

// std::_Cons_val<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>, ...>(...)
extern void __thiscall STL_ConsVal_MorphEntryPair(
    void* storage,
    MorphEntryPair* extraout_edx,
    MorphEntryPair* unaff_edi);

// CPersistTraits<C2DVector>::TransferIn(...)
extern CPersistTraits_TransferInResult __thiscall CPersistTraits_C2DVector_TransferIn(
    CPersistTraits<C2DVector>* self,
    CPersistContext* context,
    void* node_base);

CStringParser* __thiscall CPersistTraits_Transfer_And_Construct(
    CPersistTraits<C2DVector>* self,
    CCharString* key)
{
    auto* found = reinterpret_cast<std::byte*>(BinarySearchTree_FindByComparator(key));

    if (found != *reinterpret_cast<std::byte**>(self)) {
        std::less<CCharString> less_obj;
        const bool less_result = Less_CCharString(
            &less_obj,
            key,
            &reinterpret_cast<CPersistTraits_NodeKeyOverlay*>(found)->key);
        if (!less_result) {
            return &reinterpret_cast<CPersistTraits_NodeValueOverlay*>(found)->value;
        }
    }

    std::byte local_18[0x10];
    std::byte local_14[0x08];
    std::uint32_t local_c = 0;
    std::uint32_t local_8 = 0;
    MorphEntryPair* extraout_edx;
    OptimisedPrimitivePtr* extraout_edx_00;
    MorphEntryPair* unaff_edi;

    Construct_RegisteredInfoPair(
        local_18,
        key,
        reinterpret_cast<CTCBase::CRegisteredTCInfo*>(&local_c));

    const CPersistTraits_TransferInResult transfer_result =
        CPersistTraits_C2DVector_TransferIn(
            self,
            reinterpret_cast<CPersistContext*>(&key),
            found);

    auto* base = reinterpret_cast<std::byte*>(
        *reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(transfer_result.low)));

    STL_DestVal_OptimisedPrimitive(
        local_14,
        reinterpret_cast<OptimisedPrimitivePtr*>(static_cast<std::uintptr_t>(transfer_result.high)));

    STL_ConsVal_MorphEntryPair(local_18, extraout_edx, unaff_edi);

    STL_DestVal_OptimisedPrimitive(&local_c, extraout_edx_00);

    return &reinterpret_cast<CPersistTraits_NodeValueOverlay*>(base)->value;
}