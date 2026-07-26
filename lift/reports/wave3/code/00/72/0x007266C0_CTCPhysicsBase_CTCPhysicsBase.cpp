#include <cstddef>
#include <cstdint>

extern void* PTR__vector_deleting_destructor__01266314;

namespace
{
struct CTCPhysicsBaseCtorOverlay
{
    void* vftable;                    // +0x00
    std::byte pad04[0x88 - 0x04];
    std::uint8_t byte_88;             // +0x88
    std::byte pad89[0x95 - 0x89];
    std::uint32_t dword_95;           // +0x95
    std::uint32_t dword_99;           // +0x99
    std::uint32_t dword_9D;           // +0x9D
    std::byte padA1[0xAD - 0xA1];
    std::uint8_t byte_AD;             // +0xAD
};

static_assert(offsetof(CTCPhysicsBaseCtorOverlay, vftable) == 0x00);
static_assert(offsetof(CTCPhysicsBaseCtorOverlay, byte_88) == 0x88);
static_assert(offsetof(CTCPhysicsBaseCtorOverlay, dword_95) == 0x95);
static_assert(offsetof(CTCPhysicsBaseCtorOverlay, dword_99) == 0x99);
static_assert(offsetof(CTCPhysicsBaseCtorOverlay, dword_9D) == 0x9D);
static_assert(offsetof(CTCPhysicsBaseCtorOverlay, byte_AD) == 0xAD);
}

CTCPhysicsBase::CTCPhysicsBase(CThing& thing)
    : CTCActivationTrigger(thing)
{
    auto* const overlay = reinterpret_cast<CTCPhysicsBaseCtorOverlay*>(this);

    overlay->byte_88 = 0;
    overlay->vftable = &PTR__vector_deleting_destructor__01266314;
    overlay->dword_95 = 0;
    overlay->dword_99 = 0;
    overlay->dword_9D = 0;
    overlay->byte_AD = static_cast<std::uint8_t>(overlay->byte_AD & 0xFC);
}