#include <cstddef>
#include <cstdint>
#include <new>

// RE_AGENT_SEMANTIC_REVIEW: structural PASS only. The text-bank temporary
// lifetimes and helper identities remain ambiguous, and m_pField04/DoSizeof is
// not a credible final type for the random-selection state.

class CBankFile;
class CCharString;
class CGameScriptInterface;
class CPlayerGui;
class CVertexBufferWin32;
class CWideString;

enum ETextGroupSelectionMethod : std::int32_t
{
};

struct CCharStringData
{
    const char* m_pString;     // 0x00
    std::uint32_t m_StringLen; // 0x04
};

static_assert(offsetof(CCharStringData, m_pString) == 0x00);
static_assert(offsetof(CCharStringData, m_StringLen) == 0x04);
static_assert(sizeof(CCharStringData) == 0x08);

class CCharString
{
public:
    CCharString();
    ~CCharString();

    CCharStringData* m_pData; // 0x00
};

static_assert(offsetof(CCharString, m_pData) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CWideString
{
public:
    CWideString& __thiscall operator=(const CWideString& other);

    void* m_pData; // 0x00
};

static_assert(sizeof(CWideString) == 0x04);

struct CCRC
{
    static std::uint32_t __fastcall Calc(
        std::uint32_t seed,
        const void* pData,
        std::uint32_t length);
};

class CBankFile
{
public:
    std::uint32_t __thiscall GetEntryDataType(std::uint32_t entryIndex) const;
};

class CPlayerGui
{
public:
    void __thiscall AddScreenMessage(const CWideString& text);
};

class CVertexBufferWin32
{
public:
    unsigned long* __thiscall DoSizeof();
};

namespace NGameText
{
class CDataBank : public CBankFile
{
public:
    void* __thiscall GetPGroupEntry(void* pOutHandle, std::uint8_t entryIndex);
};
}

extern "C" std::uint32_t __thiscall CVectorMap__LookupValue(
    NGameText::CDataBank* pThis,
    std::uint32_t key);
extern "C" char __thiscall CWideString__NotEqual(
    const CWideString* pThis,
    const wchar_t* pOther);
extern "C" void __cdecl GFRandom(std::uint32_t maxValue, unsigned long* pState);

extern std::uint8_t DAT_0129aaf4;
extern const wchar_t DAT_0122d70c[];
extern std::uint8_t DAT_01239edc;
extern void* DAT_013b86a0;
extern CPlayerGui* DAT_013b8790;

namespace
{
struct CRefCountedReleaseOverlay
{
    void** m_pVftable;       // 0x00
    std::int32_t m_RefCount; // 0x04
};

static_assert(offsetof(CRefCountedReleaseOverlay, m_pVftable) == 0x00);
static_assert(offsetof(CRefCountedReleaseOverlay, m_RefCount) == 0x04);
static_assert(sizeof(CRefCountedReleaseOverlay) == 0x08);

struct CTextGroupEntryOverlay
{
    std::byte m_Pad00[0x1C];
    const std::uint32_t* m_pBegin; // 0x1C
    const std::uint32_t* m_pEnd;   // 0x20
};

static_assert(offsetof(CTextGroupEntryOverlay, m_pBegin) == 0x1C);
static_assert(offsetof(CTextGroupEntryOverlay, m_pEnd) == 0x20);

struct CGameTextGlobalsOverlay
{
    std::byte m_Pad00[0x14];
    NGameText::CDataBank* m_pDataBank; // 0x14
};

static_assert(offsetof(CGameTextGlobalsOverlay, m_pDataBank) == 0x14);

struct CGameScriptInterfaceOverlay
{
    void* m_pVftable;               // 0x00
    CVertexBufferWin32* m_pField04; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pField04) == 0x04);

struct COpaqueStackTemp
{
    void* m_pUnknown; // 0x00

    ~COpaqueStackTemp();
};

static_assert(offsetof(COpaqueStackTemp, m_pUnknown) == 0x00);
static_assert(sizeof(COpaqueStackTemp) == 0x04);

union CMessageTextStorage
{
    CCharString m_CharString;
    CWideString m_WideString;

    CMessageTextStorage()
    {
    }

    ~CMessageTextStorage()
    {
    }
};

static_assert(sizeof(CMessageTextStorage) == 0x04);

extern "C" std::uint32_t __thiscall CPersistContext__GetDefaultVal_CAnimationSet(
    const void* pThisLike,
    ...);
extern "C" CWideString* __fastcall CCharString__SetFromEnum(
    COpaqueStackTemp* pStackSlot18,
    const void* pUnknownEdx,
    std::uint32_t value);
extern "C" CWideString* __thiscall CBankFile__GetBankHandle(
    CBankFile* pThis,
    COpaqueStackTemp* pStackSlot18);

inline void ReleaseRefCountedObject(void* pObject)
{
    if (pObject != nullptr)
    {
        auto* const pOverlay = reinterpret_cast<CRefCountedReleaseOverlay*>(pObject);
        const std::int32_t newRefCount = pOverlay->m_RefCount - 1;
        pOverlay->m_RefCount = newRefCount;
        if (newRefCount == 0)
        {
            using RawReleaseFn = void(__thiscall*)(void*);
            reinterpret_cast<RawReleaseFn>(pOverlay->m_pVftable[1])(pObject);
        }
    }
}
}

class CGameScriptInterface
{
public:
    void __thiscall AddScreenMessage(
        const CCharString& textGroupName,
        ETextGroupSelectionMethod selectionMethod) const;
};

void __thiscall CGameScriptInterface::AddScreenMessage(
    const CCharString& textGroupName,
    ETextGroupSelectionMethod selectionMethod) const
{
    CMessageTextStorage local_4;
    new (&local_4.m_CharString) CCharString();

    NGameText::CDataBank* const pDataBank =
        reinterpret_cast<CGameTextGlobalsOverlay*>(DAT_013b86a0)->m_pDataBank;

    const void* pBytes;
    std::uint32_t byteCount;
    if (textGroupName.m_pData == nullptr)
    {
        byteCount = 0;
        pBytes = &DAT_0129aaf4;
    }
    else
    {
        byteCount = textGroupName.m_pData->m_StringLen;
        pBytes = textGroupName.m_pData->m_pString;
    }

    std::uint32_t entryIndex = CCRC::Calc(0, pBytes, byteCount);
    entryIndex = CVectorMap__LookupValue(pDataBank, entryIndex);

    if (entryIndex == 0)
    {
        alignas(COpaqueStackTemp) std::byte stackSlot18Storage[sizeof(COpaqueStackTemp)];
        alignas(COpaqueStackTemp) std::byte stackSlot1CStorage[sizeof(COpaqueStackTemp)];

        auto* const pStackSlot18 =
            reinterpret_cast<COpaqueStackTemp*>(stackSlot18Storage);
        auto* const pStackSlot1C =
            reinterpret_cast<COpaqueStackTemp*>(stackSlot1CStorage);

        const std::uint32_t defaultValue =
            CPersistContext__GetDefaultVal_CAnimationSet(&textGroupName, pStackSlot1C);
        CWideString* const pWideString =
            CCharString__SetFromEnum(pStackSlot18, &DAT_01239edc, defaultValue);

        local_4.m_WideString = *pWideString;

        pStackSlot18->~COpaqueStackTemp();
        pStackSlot1C->~COpaqueStackTemp();
    }
    else
    {
        const std::uint32_t entryDataType = pDataBank->GetEntryDataType(entryIndex);
        if (entryDataType == 0)
        {
            void* pGroupHandle = nullptr;
            CBankFile** const ppGroupEntry = reinterpret_cast<CBankFile**>(
                pDataBank->GetPGroupEntry(&pGroupHandle, static_cast<std::uint8_t>(entryIndex)));

            alignas(COpaqueStackTemp) std::byte stackSlot18Storage[sizeof(COpaqueStackTemp)];
            auto* const pStackSlot18 =
                reinterpret_cast<COpaqueStackTemp*>(stackSlot18Storage);

            CWideString* const pBankHandle =
                CBankFile__GetBankHandle(*ppGroupEntry, pStackSlot18);

            local_4.m_WideString = *pBankHandle;

            pStackSlot18->~COpaqueStackTemp();
            ReleaseRefCountedObject(pGroupHandle);
        }
        else
        {
            void* pGroupHandle = nullptr;
            CTextGroupEntryOverlay** const ppGroupEntry =
                reinterpret_cast<CTextGroupEntryOverlay**>(
                    pDataBank->GetPGroupEntry(&pGroupHandle, static_cast<std::uint8_t>(entryIndex)));
            CTextGroupEntryOverlay* const pGroupEntry = *ppGroupEntry;

            ReleaseRefCountedObject(pGroupHandle);

            const std::uint32_t entryCount = static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(pGroupEntry->m_pEnd) -
                 reinterpret_cast<std::uintptr_t>(pGroupEntry->m_pBegin)) >>
                2);

            if ((1 < entryCount) &&
                (selectionMethod != static_cast<ETextGroupSelectionMethod>(0)))
            {
                unsigned long* const pRandomState =
                    reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)
                        ->m_pField04
                        ->DoSizeof();
                GFRandom(entryCount, pRandomState);
            }
        }
    }

    if (CWideString__NotEqual(&local_4.m_WideString, DAT_0122d70c) != '\0')
    {
        DAT_013b8790->AddScreenMessage(local_4.m_WideString);
    }

    local_4.m_CharString.~CCharString();
}
