#include <cstddef>
#include <cstdint>

class CDiskFileWin32;
class CGameEventPackageSet;
class CMainGameComponent;
class CNavSwitchableLeafNode;
class CXMVPlaybackInfo;

namespace
{
struct CMainGameComponent_AddEventPackageSetToSaveOverlay
{
    std::byte Pad00[0x16154];
    CDiskFileWin32 EventPackageFile;
};

static_assert(
    offsetof(CMainGameComponent_AddEventPackageSetToSaveOverlay, EventPackageFile) == 0x16154);

using CNavSwitchableLeafNode_GetRegionFn =
    bool(__thiscall*)(CNavSwitchableLeafNode*, void**);
using CGameEventPackageSet_CompressIntoBufferFn =
    std::int32_t(__thiscall*)(const CGameEventPackageSet*, unsigned char*);
using CXMVPlaybackInfo_CXMVPlaybackInfoFn = void(__thiscall*)(CXMVPlaybackInfo*);
using EventPackageFileWriteFn =
    void(__thiscall*)(CDiskFileWin32*, void*, std::int32_t, std::int32_t);

constexpr auto CNavSwitchableLeafNode_GetRegion =
    reinterpret_cast<CNavSwitchableLeafNode_GetRegionFn>(0x009F4980);
constexpr auto CGameEventPackageSet_CompressIntoBuffer =
    reinterpret_cast<CGameEventPackageSet_CompressIntoBufferFn>(0x009F19A0);
constexpr auto CXMVPlaybackInfo_CXMVPlaybackInfo =
    reinterpret_cast<CXMVPlaybackInfo_CXMVPlaybackInfoFn>(0x009F49A0);

constexpr auto DAT_013CB498 = reinterpret_cast<CNavSwitchableLeafNode*>(0x013CB498);
}

void CMainGameComponent::AddEventPackageSetToSave(const CGameEventPackageSet* set)
{
    auto* const self =
        reinterpret_cast<CMainGameComponent_AddEventPackageSetToSaveOverlay*>(this);

    void* compressed_buffer = nullptr;

    if (CNavSwitchableLeafNode_GetRegion(DAT_013CB498, &compressed_buffer)) {
        std::int32_t compressed_size =
            CGameEventPackageSet_CompressIntoBuffer(
                set,
                reinterpret_cast<unsigned char*>(compressed_buffer));

        reinterpret_cast<EventPackageFileWriteFn>(
            (*reinterpret_cast<void***>(&self->EventPackageFile))[4])(
            &self->EventPackageFile,
            &compressed_size,
            4,
            0);

        reinterpret_cast<EventPackageFileWriteFn>(
            (*reinterpret_cast<void***>(&self->EventPackageFile))[4])(
            &self->EventPackageFile,
            compressed_buffer,
            compressed_size,
            0);

        CXMVPlaybackInfo_CXMVPlaybackInfo(reinterpret_cast<CXMVPlaybackInfo*>(DAT_013CB498));
    }
}