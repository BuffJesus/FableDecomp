#include <cstddef>
#include <cstdint>

struct CDataOutputStream;
struct CBlendedParticleEffectSet;
struct CMemoryAllocatorVariableSize;

struct SerializationParticleEffectOverlay {
    std::uint32_t allocation_list;
    std::uint32_t effect_kind;
};
static_assert(offsetof(SerializationParticleEffectOverlay, allocation_list) == 0x0);
static_assert(offsetof(SerializationParticleEffectOverlay, effect_kind) == 0x4);

struct AllocationListHeadOverlay {
    std::byte _pad0[0x8];
    std::uint32_t first_allocated_area;
};
static_assert(offsetof(AllocationListHeadOverlay, first_allocated_area) == 0x8);

struct ParticleEffectAreaOverlay {
    std::byte _pad0[0x10];
    std::uint32_t area_kind;
};
static_assert(offsetof(ParticleEffectAreaOverlay, area_kind) == 0x10);

void __fastcall Serialization_WriteParticleEffect(
    CDataOutputStream* stream,
    unsigned long* particle_effect_raw)
{
    const auto* const particle_effect =
        reinterpret_cast<const SerializationParticleEffectOverlay*>(particle_effect_raw);

    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(particle_effect->effect_kind));

    std::uint32_t current_area =
        reinterpret_cast<const AllocationListHeadOverlay*>(particle_effect->allocation_list)->first_allocated_area;

    if (current_area != particle_effect->allocation_list) {
        do {
            const auto* const area = reinterpret_cast<const ParticleEffectAreaOverlay*>(current_area);

            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(area->area_kind));
            CBlendedParticleEffectSet::TransferBinaryOut(
                reinterpret_cast<CBlendedParticleEffectSet*>(current_area + 0x14),
                stream);

            current_area = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
                reinterpret_cast<CMemoryAllocatorVariableSize*>(current_area));
        } while (current_area != particle_effect->allocation_list);
    }
}