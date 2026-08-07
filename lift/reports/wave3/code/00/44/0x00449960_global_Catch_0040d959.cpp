#include <cstddef>
#include <cstdint>

struct CPlayerManager_PlayerIndexOverlay
{
    std::byte pad_0[0x1C];
    std::int32_t playerIndex;
};
static_assert(offsetof(CPlayerManager_PlayerIndexOverlay, playerIndex) == 0x1C);

void __fastcall Catch_0040d959(CPlayerManager* self)
{
    const auto* overlay = reinterpret_cast<const CPlayerManager_PlayerIndexOverlay*>(self);
    CPlayerManager::GetPlayer(self, overlay->playerIndex);
}