#include <cstddef>
#include <cstdint>

class CWorld
{
public:
    static void __thiscall SetMiniMapEnabled(CWorld* pWorld, bool enabled);
};

class CCharString
{
public:
    void __thiscall AssignFromWide(const wchar_t* pText);
    ~CCharString();

    void* m_pRep; // 0x00
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

namespace NThingManager::CEntityRuntimePersistence
{
class CEntityScriptInfo
{
public:
    CEntityScriptInfo(CWorld* pWorld);

private:
    void* m_pVftable;                 // 0x00
    std::byte m_Pad04[0x20 - 0x04];
};

static_assert(sizeof(CEntityScriptInfo) == 0x20);
}

extern "C" void* __cdecl operator_new(std::size_t size);

extern CWorld* DAT_013b8394;

namespace
{
struct FrontendSoundScriptInfoVftableOverlay
{
    std::byte m_Pad00[0x04];
    void (__thiscall* m_RawSlot04)(void* pThis, CCharString* pPath); // 0x04
};

static_assert(offsetof(FrontendSoundScriptInfoVftableOverlay, m_RawSlot04) == 0x04);

struct FrontendSoundScriptInfoOverlay
{
    FrontendSoundScriptInfoVftableOverlay* m_pVftable; // 0x00
};

static_assert(offsetof(FrontendSoundScriptInfoOverlay, m_pVftable) == 0x00);

struct CountedPointerOverlay
{
    void* m_pObject; // 0x00

    void __thiscall Reset(void* pObject);
};

static_assert(offsetof(CountedPointerOverlay, m_pObject) == 0x00);

struct InitFrontendSoundsOwnerOverlay
{
    std::byte m_Pad00[0x50];
    CountedPointerOverlay m_FrontendSounds; // 0x50
};

static_assert(offsetof(InitFrontendSoundsOwnerOverlay, m_FrontendSounds) == 0x50);

struct FrontendWorldLikeVftableOverlay
{
    std::byte m_Pad00[0xB8];
    void (__thiscall* m_RawSlotB8)(void* pThis, std::uint32_t flags, float one, float zero); // 0xB8
};

static_assert(offsetof(FrontendWorldLikeVftableOverlay, m_RawSlotB8) == 0xB8);

struct FrontendWorldLikeOverlay
{
    FrontendWorldLikeVftableOverlay* m_pVftable; // 0x00
};

static_assert(offsetof(FrontendWorldLikeOverlay, m_pVftable) == 0x00);

union Local8Overlay
{
    int m_Param;
    CCharString m_String;

    Local8Overlay() {}
    ~Local8Overlay() {}
};

static_assert(sizeof(Local8Overlay) == 0x04);
}

void __fastcall Init_FrontendSounds(void* pOwner)
{
    if (DAT_013b8394 != nullptr)
    {
        Local8Overlay local_8;
        local_8.m_Param = static_cast<int>(reinterpret_cast<std::uintptr_t>(pOwner));

        void* pScriptInfoStorage = operator_new(0x20);
        FrontendSoundScriptInfoOverlay* pScriptInfo;
        if (pScriptInfoStorage == nullptr)
        {
            pScriptInfo = nullptr;
        }
        else
        {
            pScriptInfo = reinterpret_cast<FrontendSoundScriptInfoOverlay*>(
                new (pScriptInfoStorage)
                    NThingManager::CEntityRuntimePersistence::CEntityScriptInfo(DAT_013b8394));
        }

        reinterpret_cast<InitFrontendSoundsOwnerOverlay*>(pOwner)->m_FrontendSounds.Reset(pScriptInfo);

        local_8.m_String.AssignFromWide(L"data\\sound\\frontend.lug");
        {
            auto* const pFrontendSounds =
                &reinterpret_cast<InitFrontendSoundsOwnerOverlay*>(pOwner)->m_FrontendSounds;
            auto* const pScriptInfoObject =
                reinterpret_cast<FrontendSoundScriptInfoOverlay*>(pFrontendSounds->m_pObject);
            pScriptInfoObject->m_pVftable->m_RawSlot04(pScriptInfoObject, &local_8.m_String);
        }
        local_8.m_String.~CCharString();

        auto* const pWorldLike = reinterpret_cast<FrontendWorldLikeOverlay*>(DAT_013b8394);
        pWorldLike->m_pVftable->m_RawSlotB8(DAT_013b8394, 0x8000, 1.0f, 0.0f);
        CWorld::SetMiniMapEnabled(DAT_013b8394, true);
    }
}