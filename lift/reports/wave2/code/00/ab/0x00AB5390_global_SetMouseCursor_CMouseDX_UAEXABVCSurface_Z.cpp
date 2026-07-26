#include <cstddef>
#include <cstdint>
#include <new>

struct CIPhysicsFacesSegmentBase;

struct CSurface {
    std::byte pad_0000[0x10];
    CSurface(const CSurface&);
};

struct CountedSurfaceRep_Overlay {
    std::byte pad_0000[0x4];
    std::uint32_t value;
};

struct CCountedPointer_CIPhysicsFacesSegmentBase_const__Overlay {
    std::uint32_t rep;
};

struct CEnginePrimitive_Overlay {
    std::uint32_t vftable;
};

struct PrimitiveDispatchTable_Overlay {
    std::byte pad_0000[0x28];
    std::uint32_t fn_28;
    std::byte pad_002C[0x4];
    std::uint32_t fn_30;
};

struct PrimitiveSlotOwner_Overlay {
    std::byte pad_0000[0x58];
    std::uint32_t primitive;
};

struct PrimitiveAddChildResult_Overlay {
    std::byte pad_0000[0x60];
    std::uint32_t owner;
};

struct CMouseDX_SetMouseCursor_Overlay {
    std::byte pad_0000[0x343C];
    std::uint8_t flag_343C;
    std::byte pad_343D[0x141B];
    CCountedPointer_CIPhysicsFacesSegmentBase_const__Overlay field_4858;
};

extern void* __cdecl operator_new(std::size_t size);

extern void __thiscall Reset_CountedPointer_CIPhysicsFacesSegmentBase_const_(
    CCountedPointer_CIPhysicsFacesSegmentBase_const__Overlay* self,
    CIPhysicsFacesSegmentBase* value);

// Observed direct-call shape using carried incoming register state.
extern std::uint32_t __cdecl AddChildPrimitive_WithOriginalCarriedState();

// Observed second call shape:
//   AddChildPrimitive(this_01, pCVar4, uVar3)
// with `uVar3` explicitly zeroed beforehand and `this_01` carried in.
extern std::uint32_t __cdecl AddChildPrimitive_WithCarriedThisPrimitiveAndValue(
    std::uint32_t carriedThis01,
    std::uint32_t carriedPrimitive,
    std::uint32_t carriedValue);

// Opaque capture of the carried `this_01` input used by the second direct call.
extern std::uint32_t __cdecl GetAddChildPrimitiveSecondThisCarriedState();

static_assert(sizeof(CSurface) == 0x10);
static_assert(sizeof(CCountedPointer_CIPhysicsFacesSegmentBase_const__Overlay) == 0x4);
static_assert(sizeof(CEnginePrimitive_Overlay) == 0x4);
static_assert(sizeof(PrimitiveSlotOwner_Overlay) == 0x5C);
static_assert(sizeof(PrimitiveAddChildResult_Overlay) == 0x64);
static_assert(offsetof(CountedSurfaceRep_Overlay, value) == 0x4);
static_assert(offsetof(PrimitiveDispatchTable_Overlay, fn_28) == 0x28);
static_assert(offsetof(PrimitiveDispatchTable_Overlay, fn_30) == 0x30);
static_assert(offsetof(PrimitiveSlotOwner_Overlay, primitive) == 0x58);
static_assert(offsetof(PrimitiveAddChildResult_Overlay, owner) == 0x60);
static_assert(offsetof(CMouseDX_SetMouseCursor_Overlay, flag_343C) == 0x343C);
static_assert(offsetof(CMouseDX_SetMouseCursor_Overlay, field_4858) == 0x4858);

template <typename T>
static T* Ptr32(std::uint32_t address)
{
    return reinterpret_cast<T*>(static_cast<std::uintptr_t>(address));
}

static std::uint32_t PtrTo32(const void* ptr)
{
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(ptr));
}

void CMouseDX::SetMouseCursor(const CSurface& surface)
{
    auto* self = reinterpret_cast<CMouseDX_SetMouseCursor_Overlay*>(this);

    auto* surfaceStorage = static_cast<CSurface*>(operator_new(0x10));
    CIPhysicsFacesSegmentBase* copiedSurface;
    if (surfaceStorage == nullptr) {
        copiedSurface = nullptr;
    }
    else {
        copiedSurface = reinterpret_cast<CIPhysicsFacesSegmentBase*>(
            ::new (surfaceStorage) CSurface(surface));
    }

    Reset_CountedPointer_CIPhysicsFacesSegmentBase_const_(&self->field_4858, copiedSurface);

    if (self->flag_343C != 0) {
        using PrimitiveFn28 = void(__thiscall*)(CEnginePrimitive_Overlay*, int, int, std::uint32_t);
        using PrimitiveFn30 = void(__thiscall*)(CEnginePrimitive_Overlay*, int);

        std::uint32_t addChildResult = AddChildPrimitive_WithOriginalCarriedState();
        auto* primitive =
            Ptr32<CEnginePrimitive_Overlay>(Ptr32<PrimitiveSlotOwner_Overlay>(
                Ptr32<PrimitiveAddChildResult_Overlay>(addChildResult)->owner)->primitive);
        auto* dispatch = Ptr32<PrimitiveDispatchTable_Overlay>(primitive->vftable);
        auto* rep = Ptr32<CountedSurfaceRep_Overlay>(self->field_4858.rep);
        std::uint32_t carriedThis01 = GetAddChildPrimitiveSecondThisCarriedState();
        std::uint32_t carriedValue = 0;

        reinterpret_cast<PrimitiveFn28>(static_cast<std::uintptr_t>(dispatch->fn_28))(
            primitive,
            0,
            0,
            rep->value);

        addChildResult = AddChildPrimitive_WithCarriedThisPrimitiveAndValue(
            carriedThis01,
            PtrTo32(primitive),
            carriedValue);
        primitive =
            Ptr32<CEnginePrimitive_Overlay>(Ptr32<PrimitiveSlotOwner_Overlay>(
                Ptr32<PrimitiveAddChildResult_Overlay>(addChildResult)->owner)->primitive);
        dispatch = Ptr32<PrimitiveDispatchTable_Overlay>(primitive->vftable);

        reinterpret_cast<PrimitiveFn30>(static_cast<std::uintptr_t>(dispatch->fn_30))(
            primitive,
            1);
    }
}