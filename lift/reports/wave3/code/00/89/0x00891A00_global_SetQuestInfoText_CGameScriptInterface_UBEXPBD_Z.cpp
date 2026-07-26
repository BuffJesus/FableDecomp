#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

class CGameScriptInterface;
class CCharString;
class CWideString;
class CBankFile;

class CParticleMorphs
{
public:
    class CEntry;
};

enum EHeroMorphType : std::int32_t
{
};

namespace NGameText
{
class CDataBank
{
public:
    void __thiscall GetPGroupEntry(CBankFile** ppBankFile, unsigned long entryIndex, bool load);
};
}

class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CWideString
{
public:
    CWideString& __thiscall operator=(const CWideString& other);
};

static_assert(sizeof(CWideString) == 0x04);

class CBankFile
{
public:
    CWideString* __thiscall GetBankHandle();
};

class CGameScriptInterface
{
public:
    void __thiscall SetQuestInfoText(const char* param_1) const;
};

namespace std
{
template <class Allocator, class ValueType, class ReferenceType>
void __thiscall _Cons_val(
    Allocator* pAllocator,
    const ValueType* pValue,
    const ValueType* pUnkEdi);
}

extern std::uint8_t DAT_0129aaf4;
extern void* DAT_013b86a0;
extern void* DAT_013b8790;

struct CCRC
{
    static std::uint32_t __fastcall Calc(CCRC* pThis, const void* pData, std::uint32_t length);
};

namespace
{
using MorphEntryPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using MorphEntryAllocator = std::allocator<MorphEntryPair>;

struct CCharStringStorage
{
    const char* m_pData;    // 0x00
    std::uint32_t m_Length; // 0x04
};

static_assert(offsetof(CCharStringStorage, m_pData) == 0x00);
static_assert(offsetof(CCharStringStorage, m_Length) == 0x04);
static_assert(sizeof(CCharStringStorage) == 0x08);

struct CCharStringOverlay
{
    CCharStringStorage* m_pStorage; // 0x00
};

static_assert(offsetof(CCharStringOverlay, m_pStorage) == 0x00);
static_assert(sizeof(CCharStringOverlay) == 0x04);

struct QuestInfoGlobalsOverlay
{
    std::byte m_Pad00[0x14];
    NGameText::CDataBank* m_pDataBank; // 0x14
};

static_assert(offsetof(QuestInfoGlobalsOverlay, m_pDataBank) == 0x14);

struct QuestInfoGuiGlobalsOverlay
{
    std::byte m_Pad00[0x1C8];
    void* m_pDrawQuestInfo; // 0x1C8
};

static_assert(offsetof(QuestInfoGuiGlobalsOverlay, m_pDrawQuestInfo) == 0x1C8);

struct DrawQuestInfoTextOverlay
{
    std::byte m_Pad00[0x54];
    CWideString m_QuestInfoText; // 0x54
};

static_assert(offsetof(DrawQuestInfoTextOverlay, m_QuestInfoText) == 0x54);

struct CBankFileVTableOverlay
{
    void* m_Slot00;                                   // 0x00
    void (__thiscall* m_RawSlot04)(CBankFile* pThis); // 0x04
};

static_assert(offsetof(CBankFileVTableOverlay, m_RawSlot04) == 0x04);

struct CBankFileOverlay
{
    const CBankFileVTableOverlay* m_pVTable; // 0x00
    std::int32_t m_RefCount;                 // 0x04
};

static_assert(offsetof(CBankFileOverlay, m_pVTable) == 0x00);
static_assert(offsetof(CBankFileOverlay, m_RefCount) == 0x04);

extern "C" std::uint64_t __thiscall CVectorMap__LookupValue(
    NGameText::CDataBank* pThis,
    std::uint32_t crc);
}

void __thiscall CGameScriptInterface::SetQuestInfoText(const char* param_1) const
{
    MorphEntryPair* unaff_EDI;
    std::byte local_4[sizeof(CCharString)];
    const char* const pText = param_1;

    auto* const pDataBank =
        reinterpret_cast<QuestInfoGlobalsOverlay*>(DAT_013b86a0)->m_pDataBank;

    new (const_cast<char**>(&param_1)) CCharString(pText, -1);

    const auto* const pString =
        reinterpret_cast<const CCharStringOverlay*>(&param_1);

    std::uint32_t uVar5;
    const void* puVar3;
    if (pString->m_pStorage == nullptr)
    {
        uVar5 = 0;
        puVar3 = &DAT_0129aaf4;
    }
    else
    {
        uVar5 = pString->m_pStorage->m_Length;
        puVar3 = pString->m_pStorage->m_pData;
    }

    uVar5 = CCRC::Calc(nullptr, puVar3, uVar5);
    const std::uint64_t uVar6 = CVectorMap__LookupValue(pDataBank, uVar5);

    std::_Cons_val<MorphEntryAllocator, MorphEntryPair, const MorphEntryPair&>(
        reinterpret_cast<MorphEntryAllocator*>(&param_1),
        reinterpret_cast<const MorphEntryPair*>(static_cast<std::uintptr_t>(uVar6 >> 32)),
        unaff_EDI);

    if (static_cast<std::int32_t>(uVar6) != 0)
    {
        pDataBank->GetPGroupEntry(
            reinterpret_cast<CBankFile**>(&param_1),
            static_cast<unsigned long>(uVar6),
            true);

        auto* const pDrawQuestInfo =
            reinterpret_cast<DrawQuestInfoTextOverlay*>(
                reinterpret_cast<QuestInfoGuiGlobalsOverlay*>(DAT_013b8790)->m_pDrawQuestInfo);

        pDrawQuestInfo->m_QuestInfoText =
            *reinterpret_cast<CBankFile*>(param_1)->GetBankHandle();

        reinterpret_cast<CCharString*>(local_4)->~CCharString();

        if (reinterpret_cast<CBankFile*>(param_1) != nullptr)
        {
            auto* const pBankFile =
                reinterpret_cast<CBankFileOverlay*>(reinterpret_cast<CBankFile*>(param_1));
            const std::int32_t iVar1 = pBankFile->m_RefCount;
            pBankFile->m_RefCount = iVar1 - 1;
            if (iVar1 - 1 == 0)
            {
                pBankFile->m_pVTable->m_RawSlot04(reinterpret_cast<CBankFile*>(param_1));
            }
        }
    }
}