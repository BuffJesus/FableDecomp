#include <cstddef>
#include <cstdint>

class CCharString;
class CCodeSectionManager;
class CDataBank;
class CIEngine;
class CWideString;
class CXMVPlayerBuffers;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

class CGameScriptInterface
{
public:
    virtual void __thiscall PlayAVIMovie(const CCharString& movieName) const;
};

using _func_bool = bool(__cdecl*)();

void __fastcall GFPlayFullScreenXBoxMovie(
    const CCharString* pMovieName,
    std::int32_t movieType,
    std::int32_t arg3,
    std::int32_t arg4,
    std::int32_t arg5,
    CDataBank* pDataBank,
    CIEngine* pEngine,
    bool enabled,
    float value,
    _func_bool pCallback,
    CXMVPlayerBuffers* pBuffers,
    CWideString* pWideString);

namespace
{
struct CCodeSectionManager_PlayAVIMovie_Overlay
{
    std::byte m_Pad00[0x0C];
    std::uint8_t m_Field0C; // 0x0C
    std::uint8_t m_Field0D; // 0x0D
    std::byte m_Pad0E[0xCD - 0x0E];
    std::uint8_t m_FieldCD; // 0xCD
};

static_assert(
    offsetof(CCodeSectionManager_PlayAVIMovie_Overlay, m_Field0C) == 0x0C);
static_assert(
    offsetof(CCodeSectionManager_PlayAVIMovie_Overlay, m_Field0D) == 0x0D);
static_assert(
    offsetof(CCodeSectionManager_PlayAVIMovie_Overlay, m_FieldCD) == 0xCD);
}

void __thiscall CGameScriptInterface::PlayAVIMovie(
    const CCharString& movieName) const
{
    const auto* const pCodeSectionManager =
        reinterpret_cast<const CCodeSectionManager_PlayAVIMovie_Overlay*>(
            CTBaseSingleton<CCodeSectionManager>::Get());

    std::uint8_t movieFlag;
    if (pCodeSectionManager->m_Field0C == 0)
    {
        movieFlag = pCodeSectionManager->m_Field0D;
    }
    else
    {
        movieFlag = pCodeSectionManager->m_FieldCD;
    }

    GFPlayFullScreenXBoxMovie(
        &movieName,
        0x1B,
        0,
        0,
        0,
        nullptr,
        nullptr,
        static_cast<bool>(movieFlag),
        -1.0f,
        nullptr,
        nullptr,
        nullptr);
}