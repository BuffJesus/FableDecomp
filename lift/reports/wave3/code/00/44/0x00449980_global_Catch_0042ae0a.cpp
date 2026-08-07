#include <cstddef>
#include <cstdint>

struct PlayerManagerCatchOverlay {
    std::byte pad_00[0x1C];
    long currentPlayerIndex;
};
static_assert(offsetof(PlayerManagerCatchOverlay, currentPlayerIndex) == 0x1C);

void __fastcall Catch_0042ae0a(CPlayerManager* playerManager) {
    const auto* const overlay =
        reinterpret_cast<const PlayerManagerCatchOverlay*>(playerManager);

    CPlayer* const player =
        CPlayerManager::GetPlayer(playerManager, overlay->currentPlayerIndex);

    CWorld::DrawGetEnvironment(reinterpret_cast<CWorld*>(player));
}