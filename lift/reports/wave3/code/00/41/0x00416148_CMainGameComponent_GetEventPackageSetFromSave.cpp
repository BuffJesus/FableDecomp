#include <cstddef>
#include <cstdint>

class CDiskFileWin32;
class CGameEventPackageSet;
class CMainGameComponent;
class CNavRegion;
class CNavSwitchableLeafNode;
class CXMVPlaybackInfo;

namespace
{
struct CMainGameComponent_GetEventPackageSetFromSaveOverlay
{
    std::byte Pad00[0x16154];
    CDiskFileWin32 EventPackageFile;
};

static_assert(
    offsetof(CMainGameComponent_GetEventPackageSetFromSaveOverlay, EventPackageFile) == 0x16154);

using CNavSwitchableLeafNode_GetRegionFn =
    CNavRegion*(__thiscall*)(CNavSwitchableLeafNode*);
using CXMVPlaybackInfo_CXMVPlaybackInfoFn = void(__thiscall*)(CXMVPlaybackInfo*);
using EventPackageFileReadFn =
    void(__thiscall*)(CDiskFileWin32*, void*, std::int32_t, std::int32_t);

constexpr auto CNavSwitchableLeafNode_GetRegion =
    reinterpret_cast<CNavSwitchableLeafNode_GetRegionFn>(0x009F4980);
constexpr auto CXMVPlaybackInfo_CXMVPlaybackInfo =
    reinterpret_cast<CXMVPlaybackInfo_CXMVPlaybackInfoFn>(0x009F49A2);

constexpr auto DAT_013CB498 = reinterpret_cast<CNavSwitchableLeafNode*>(0x013CB498);
}

bool CMainGameComponent::GetEventPackageSetFromSave(CGameEventPackageSet* set)
{
    auto* const self =
        reinterpret_cast<CMainGameComponent_GetEventPackageSetFromSaveOverlay*>(this);

    CMainGameComponent* local_c = this;
    CMainGameComponent* local_8 = this;

    CNavRegion* const region = CNavSwitchableLeafNode_GetRegion(DAT_013CB498);
    const bool result = region != nullptr;

    if (result) {
        reinterpret_cast<EventPackageFileReadFn>(
            (*reinterpret_cast<void***>(&self->EventPackageFile))[3])(
            &self->EventPackageFile,
            &local_c,
            4,
            0);

        reinterpret_cast<EventPackageFileReadFn>(
            (*reinterpret_cast<void***>(&self->EventPackageFile))[3])(
            &self->EventPackageFile,
            local_8,
            reinterpret_cast<std::int32_t>(local_c),
            0);

        CGameEventPackageSet::InitFromCompressedBuffer(
            set,
            reinterpret_cast<unsigned char*>(local_8));

        CXMVPlaybackInfo_CXMVPlaybackInfo(reinterpret_cast<CXMVPlaybackInfo*>(DAT_013CB498));
    }

    return result;
}