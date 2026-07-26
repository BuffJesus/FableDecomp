#include <cstddef>
#include <cstdint>

class CGameEventPackage;
class CGameEventPackageSet;
class CNetworkClient;
class CWorld;
class CDisplayEngine;

namespace
{
struct CGameEventPackageFrameOverlay
{
    std::int32_t ServerFrame;
};

static_assert(offsetof(CGameEventPackageFrameOverlay, ServerFrame) == 0x00);

struct CMainGameComponent_UpdateFromEventPackageSetOverlay
{
    std::byte Pad00[0x24];
    CWorld* PWorld;
    CDisplayEngine* PDisplayEngine;
    std::byte Pad2C[0x48 - 0x2C];
    std::int32_t CurrentServerFrame;
    std::int32_t InputServerFrame;
    std::byte Pad50[0x13AB8 - 0x50];
    std::byte NetworkClientStorage;
    std::byte Pad13AB9[0x1613C - 0x13AB9];
    std::uint32_t WorldSeed;
    std::byte Pad16140[0x161E1 - 0x16140];
    std::uint8_t Unknown161E1;
};

static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, PWorld) == 0x24);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, PDisplayEngine) == 0x28);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, CurrentServerFrame) == 0x48);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, InputServerFrame) == 0x4C);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, NetworkClientStorage) == 0x13AB8);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, WorldSeed) == 0x1613C);
static_assert(offsetof(CMainGameComponent_UpdateFromEventPackageSetOverlay, Unknown161E1) == 0x161E1);

using CGameEventPackageSet_GetCountFn =
    std::uint32_t(__thiscall*)(const CGameEventPackageSet*);
using CGameEventPackageSet_GetPackageFn =
    CGameEventPackage*(__thiscall*)(const CGameEventPackageSet*, std::uint32_t);

constexpr auto CGameEventPackageSet_GetCount =
    reinterpret_cast<CGameEventPackageSet_GetCountFn>(0x009F1710);
constexpr auto CGameEventPackageSet_GetPackage =
    reinterpret_cast<CGameEventPackageSet_GetPackageFn>(0x009F1700);
}

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* event_package_set)
{
    auto* const self =
        reinterpret_cast<CMainGameComponent_UpdateFromEventPackageSetOverlay*>(this);

    std::uint32_t event_index = 0;
    std::uint32_t event_count = CGameEventPackageSet_GetCount(event_package_set);

    if (event_count != 0) {
        do {
            CGameEventPackage* const event_package =
                CGameEventPackageSet_GetPackage(event_package_set, event_index);

            if (self->InputServerFrame <
                reinterpret_cast<CGameEventPackageFrameOverlay*>(event_package)->ServerFrame) {
                ProcessEventPackage(event_package);

                bool reached_current_server_frame = false;
                if (self->InputServerFrame == self->CurrentServerFrame) {
                    reached_current_server_frame = true;
                    self->Unknown161E1 = 1;
                }

                self->PWorld->Update(event_package, reached_current_server_frame);
                self->PDisplayEngine->ConfirmFeedbackGameEventPackage(event_package);

                self->InputServerFrame =
                    reinterpret_cast<CGameEventPackageFrameOverlay*>(event_package)->ServerFrame;

                std::int32_t next_current_server_frame = self->CurrentServerFrame;
                if (self->CurrentServerFrame <=
                    reinterpret_cast<CGameEventPackageFrameOverlay*>(event_package)->ServerFrame) {
                    next_current_server_frame =
                        reinterpret_cast<CGameEventPackageFrameOverlay*>(event_package)->ServerFrame;
                }

                const std::uint32_t world_seed = self->WorldSeed;
                self->CurrentServerFrame = next_current_server_frame;

                const std::uint32_t world_checksum = PeekWorldChecksum();
                reinterpret_cast<CNetworkClient*>(&self->NetworkClientStorage)
                    ->Update(next_current_server_frame, world_checksum, world_seed);
                self->PDisplayEngine->WorldUpdate();
            }

            ++event_index;
            event_count = CGameEventPackageSet_GetCount(event_package_set);
        } while (event_index < event_count);
    }
}