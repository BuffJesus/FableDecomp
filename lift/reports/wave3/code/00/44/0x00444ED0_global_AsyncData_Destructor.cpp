#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CBankFileAsyncData
{
    static void FreeBuffer(CBankFileAsyncData* self);
};

struct AsyncDataDestructorEntry
{
    std::uint32_t raw_vftable; // 0x00
    std::uint32_t raw_04;      // 0x04
};

static_assert(sizeof(AsyncDataDestructorEntry) == 0x08);
static_assert(offsetof(AsyncDataDestructorEntry, raw_vftable) == 0x00);
static_assert(offsetof(AsyncDataDestructorEntry, raw_04) == 0x04);

struct AsyncDataDestructorOverlay
{
    std::byte pad_00[0x1C];
    std::uint32_t range_1C_begin;              // 0x1C
    std::uint32_t range_20_end;                // 0x20
    std::byte pad_24[0x50 - 0x24];
    std::uint32_t buffer_50;                   // 0x50
    std::byte pad_54[0x68 - 0x54];
    std::byte async_data_68[0x28];             // 0x68
};

static_assert(offsetof(AsyncDataDestructorOverlay, range_1C_begin) == 0x1C);
static_assert(offsetof(AsyncDataDestructorOverlay, range_20_end) == 0x20);
static_assert(offsetof(AsyncDataDestructorOverlay, buffer_50) == 0x50);
static_assert(offsetof(AsyncDataDestructorOverlay, async_data_68) == 0x68);

void __fastcall AsyncData_Destructor(void* param_1)
{
    auto* const self = reinterpret_cast<AsyncDataDestructorOverlay*>(param_1);

    CBankFileAsyncData::FreeBuffer(
        reinterpret_cast<CBankFileAsyncData*>(&self->async_data_68));

    if (self->buffer_50 != 0)
    {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->buffer_50)));
    }

    {
        auto* const end =
            reinterpret_cast<AsyncDataDestructorEntry*>(static_cast<std::uintptr_t>(self->range_20_end));

        for (auto* it =
                 reinterpret_cast<AsyncDataDestructorEntry*>(static_cast<std::uintptr_t>(self->range_1C_begin));
             it != end;
             ++it)
        {
            using RawIndirectCall = void(__thiscall*)(void*, int);
            auto* const slot =
                *reinterpret_cast<RawIndirectCall*>(static_cast<std::uintptr_t>(it->raw_vftable));
            slot(it, 0);
        }
    }

    if (self->range_1C_begin != 0)
    {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_1C_begin)));
    }
}