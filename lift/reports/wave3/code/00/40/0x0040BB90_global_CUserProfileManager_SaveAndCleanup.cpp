#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CUserProfileManager
{
    void __thiscall Save();
};

struct CUserProfileManager_SaveAndCleanupOverlay
{
    std::byte pad_00[0x8C];
    std::uint32_t field_8c;
    std::uint32_t field_90;
    std::byte pad_94[0x04];
    std::uint32_t field_98;
    void* field_9c;
    std::uint32_t field_a0;
    std::uint32_t field_a4;
    std::uint32_t field_a8;
};

static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_8c) == 0x8C);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_90) == 0x90);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_98) == 0x98);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_9c) == 0x9C);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_a0) == 0xA0);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_a4) == 0xA4);
static_assert(offsetof(CUserProfileManager_SaveAndCleanupOverlay, field_a8) == 0xA8);

void __fastcall CUserProfileManager_SaveAndCleanup(CUserProfileManager* param_1)
{
    auto& self = *reinterpret_cast<CUserProfileManager_SaveAndCleanupOverlay*>(param_1);

    self.field_90 = self.field_8c;
    param_1->Save();

    if (self.field_a8 != self.field_98) {
        do {
            if (self.field_98 == self.field_a0 - 4) {
                if (self.field_9c != nullptr) {
                    std::free(self.field_9c);
                }

                const std::uint32_t iVar1 = self.field_a4;
                self.field_a4 = iVar1 + 4;

                const std::uint32_t nextBlock =
                    *reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(iVar1 + 4));
                self.field_9c = reinterpret_cast<void*>(static_cast<std::uintptr_t>(nextBlock));
                self.field_a0 = nextBlock + 0x80;
                self.field_98 = nextBlock;
            } else {
                self.field_98 = self.field_98 + 4;
            }
        } while (self.field_a8 != self.field_98);
    }
}