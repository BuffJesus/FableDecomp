#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CCharString {
    ~CCharString();
};

struct CBankFileAsyncData {
    static void FreeBuffer(CBankFileAsyncData* self);
};

namespace NHeroInformationScreens {
struct CBase;
}

// Ground-truth terminal tailcall identified by the decompiler as
// NHeroInformationScreens::CBase::CBase(param_1). Kept unresolved here
// instead of strengthening it into a destructor call.
void __fastcall NHeroInformationScreens_CBase_CBase(NHeroInformationScreens::CBase* self);

struct CCharStringStorage {
    std::uint32_t raw;
};
static_assert(sizeof(CCharStringStorage) == 0x4);
static_assert(alignof(CCharStringStorage) == alignof(std::uint32_t));

struct CBankFileAsyncDataStorage {
    std::byte raw[0x28];
};
static_assert(sizeof(CBankFileAsyncDataStorage) == 0x28);
static_assert(alignof(CBankFileAsyncDataStorage) == 1);

struct TextureCacheDestructorOverlay {
    std::byte pad_000[0x30];
    CCharStringStorage string_30;             // 0x30
    CBankFileAsyncDataStorage async_data_34;  // 0x34
    CCharStringStorage string_5C;             // 0x5C
    CCharStringStorage string_60;             // 0x60
    CCharStringStorage string_64;             // 0x64
    CCharStringStorage string_68;             // 0x68
    CCharStringStorage string_6C;             // 0x6C
    CCharStringStorage string_70;             // 0x70
    CCharStringStorage string_74;             // 0x74
    std::byte pad_078[0x94 - 0x78];
    std::uint32_t range_94_begin;             // 0x94
    std::uint32_t range_98_end;               // 0x98
    std::byte pad_09C[0xA0 - 0x9C];
    std::uint32_t range_A0_begin;             // 0xA0
    std::uint32_t range_A4_end;               // 0xA4
    std::byte pad_0A8[0xAC - 0xA8];
    std::uint32_t range_AC_begin;             // 0xAC
    std::uint32_t range_B0_end;               // 0xB0
    std::byte pad_0B4[0xB8 - 0xB4];
    std::uint32_t buffer_B8;                  // 0xB8
    std::byte pad_0BC[0xC4 - 0xBC];
    std::uint32_t range_C4_begin;             // 0xC4
    std::uint32_t range_C8_end;               // 0xC8
    std::byte pad_0CC[0xD0 - 0xCC];
    std::uint32_t range_D0_begin;             // 0xD0
    std::uint32_t range_D4_end;               // 0xD4
};

static_assert(offsetof(TextureCacheDestructorOverlay, string_30) == 0x30);
static_assert(offsetof(TextureCacheDestructorOverlay, async_data_34) == 0x34);
static_assert(offsetof(TextureCacheDestructorOverlay, string_5C) == 0x5C);
static_assert(offsetof(TextureCacheDestructorOverlay, string_60) == 0x60);
static_assert(offsetof(TextureCacheDestructorOverlay, string_64) == 0x64);
static_assert(offsetof(TextureCacheDestructorOverlay, string_68) == 0x68);
static_assert(offsetof(TextureCacheDestructorOverlay, string_6C) == 0x6C);
static_assert(offsetof(TextureCacheDestructorOverlay, string_70) == 0x70);
static_assert(offsetof(TextureCacheDestructorOverlay, string_74) == 0x74);
static_assert(offsetof(TextureCacheDestructorOverlay, range_94_begin) == 0x94);
static_assert(offsetof(TextureCacheDestructorOverlay, range_98_end) == 0x98);
static_assert(offsetof(TextureCacheDestructorOverlay, range_A0_begin) == 0xA0);
static_assert(offsetof(TextureCacheDestructorOverlay, range_A4_end) == 0xA4);
static_assert(offsetof(TextureCacheDestructorOverlay, range_AC_begin) == 0xAC);
static_assert(offsetof(TextureCacheDestructorOverlay, range_B0_end) == 0xB0);
static_assert(offsetof(TextureCacheDestructorOverlay, buffer_B8) == 0xB8);
static_assert(offsetof(TextureCacheDestructorOverlay, range_C4_begin) == 0xC4);
static_assert(offsetof(TextureCacheDestructorOverlay, range_C8_end) == 0xC8);
static_assert(offsetof(TextureCacheDestructorOverlay, range_D0_begin) == 0xD0);
static_assert(offsetof(TextureCacheDestructorOverlay, range_D4_end) == 0xD4);

void __fastcall TextureCache_Destructor(NHeroInformationScreens::CBase* param_1)
{
    auto* const self = reinterpret_cast<TextureCacheDestructorOverlay*>(param_1);

    {
        auto* end = reinterpret_cast<CCharStringStorage*>(static_cast<std::uintptr_t>(self->range_D4_end));
        for (auto* it = reinterpret_cast<CCharStringStorage*>(static_cast<std::uintptr_t>(self->range_D0_begin)); it != end; ++it) {
            reinterpret_cast<CCharString*>(it)->~CCharString();
        }
    }
    if (self->range_D0_begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_D0_begin)));
    }

    {
        auto* end = reinterpret_cast<std::byte*>(static_cast<std::uintptr_t>(self->range_C8_end));
        for (auto* it = reinterpret_cast<std::byte*>(static_cast<std::uintptr_t>(self->range_C4_begin)); it != end; it += 0x14) {
            reinterpret_cast<CCharString*>(it)->~CCharString();
        }
    }
    if (self->range_C4_begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_C4_begin)));
    }

    if (self->buffer_B8 != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->buffer_B8)));
    }

    {
        auto* end = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_B0_end));
        for (auto* it = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_AC_begin)); it != end; it += 2) {
            using RawIndirectCall = void(__thiscall*)(void*, int);
            auto* const object = reinterpret_cast<void*>(it);
            auto* const slot = *reinterpret_cast<RawIndirectCall*>(static_cast<std::uintptr_t>(*it));
            slot(object, 0);
        }
    }
    if (self->range_AC_begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_AC_begin)));
    }

    {
        auto* end = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_A4_end));
        for (auto* it = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_A0_begin)); it != end; it += 2) {
            using RawIndirectCall = void(__thiscall*)(void*, int);
            auto* const object = reinterpret_cast<void*>(it);
            auto* const slot = *reinterpret_cast<RawIndirectCall*>(static_cast<std::uintptr_t>(*it));
            slot(object, 0);
        }
    }
    if (self->range_A0_begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_A0_begin)));
    }

    {
        auto* end = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_98_end));
        for (auto* it = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(self->range_94_begin)); it != end; it += 2) {
            using RawIndirectCall = void(__thiscall*)(void*, int);
            auto* const object = reinterpret_cast<void*>(it);
            auto* const slot = *reinterpret_cast<RawIndirectCall*>(static_cast<std::uintptr_t>(*it));
            slot(object, 0);
        }
    }
    if (self->range_94_begin != 0) {
        std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->range_94_begin)));
    }

    reinterpret_cast<CCharString*>(&self->string_74)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_70)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_6C)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_68)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_64)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_60)->~CCharString();
    reinterpret_cast<CCharString*>(&self->string_5C)->~CCharString();
    CBankFileAsyncData::FreeBuffer(reinterpret_cast<CBankFileAsyncData*>(&self->async_data_34));
    reinterpret_cast<CCharString*>(&self->string_30)->~CCharString();
    NHeroInformationScreens_CBase_CBase(param_1);
}