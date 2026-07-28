#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class CBankFile;
class CCharString;
class CWideString;
class CGameScriptInterface;
enum EHeroMorphType : std::uint32_t;
namespace CParticleMorphs { class CEntry; }

namespace CCRC {
unsigned long Calc(unsigned long seed, const void* data, unsigned long size);
}

namespace NGameText {
class CDataBank {
public:
    void GetPGroupEntry(CBankFile** outBankFile, unsigned char foundFlag);
};
}

class CCharString {
public:
    CCharString(const char* text, int length);
    ~CCharString();
};

class CWideString {
public:
    static CWideString& operator=(CWideString* self, CWideString* rhs);
};

class CBankFile {
public:
    static CWideString* GetBankHandle(CBankFile* bankFile);
};

using ParticleMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using ParticleMorphAllocator = std::allocator<ParticleMorphPair>;

namespace std {
void __thiscall _Cons_val(
    ParticleMorphAllocator* self,
    ParticleMorphPair* value,
    ParticleMorphPair* opaqueEdi);
}

extern std::byte* DAT_013b86a0;
extern std::byte* DAT_013b8790;
extern const char DAT_0129aaf4;
extern std::uint64_t CVectorMap__LookupValue(unsigned long crc);

struct CCharStringRepOverlay {
    const char* data;
    std::uint32_t length;
};
static_assert(offsetof(CCharStringRepOverlay, data) == 0x0);
static_assert(offsetof(CCharStringRepOverlay, length) == 0x4);

struct RefCountedBankFileOverlay {
    void** vftable;
    std::int32_t refCount;
};
static_assert(offsetof(RefCountedBankFileOverlay, vftable) == 0x0);
static_assert(offsetof(RefCountedBankFileOverlay, refCount) == 0x4);

struct ParamSlotOverlay {
    std::uint32_t raw;
};
static_assert(sizeof(ParamSlotOverlay) == 0x4);

struct CCharStringLocalOverlay {
    std::uint32_t raw;
};
static_assert(sizeof(CCharStringLocalOverlay) == 0x4);

void CGameScriptInterface::SetQuestInfoName(const char* name) const
{
    NGameText::CDataBank* const dataBank =
        *reinterpret_cast<NGameText::CDataBank**>(DAT_013b86a0 + 0x14);
    ParticleMorphPair* unaff_EDI;
    ParamSlotOverlay paramSlot;
    CCharStringLocalOverlay local_4;

    CCharString::CCharString(reinterpret_cast<CCharString*>(&local_4), name, -1);

    CCharStringRepOverlay* const rep =
        *reinterpret_cast<CCharStringRepOverlay**>(&local_4);

    unsigned long length;
    const char* data;
    if (rep == nullptr) {
        length = 0;
        data = &DAT_0129aaf4;
    } else {
        length = rep->length;
        data = rep->data;
    }

    const unsigned long crc = CCRC::Calc(0, data, length);
    const std::uint64_t lookup = CVectorMap__LookupValue(crc);

    std::_Cons_val(
        reinterpret_cast<ParticleMorphAllocator*>(&paramSlot),
        reinterpret_cast<ParticleMorphPair*>(static_cast<std::uint32_t>(lookup >> 32)),
        unaff_EDI);

    if (static_cast<std::uint32_t>(lookup) != 0) {
        dataBank->GetPGroupEntry(
            reinterpret_cast<CBankFile**>(&paramSlot),
            static_cast<unsigned char>(lookup));

        const int questInfoBase = *reinterpret_cast<int*>(DAT_013b8790 + 0x1C8);
        CWideString* const bankHandle =
            CBankFile::GetBankHandle(*reinterpret_cast<CBankFile**>(&paramSlot));
        CWideString::operator=(
            reinterpret_cast<CWideString*>(questInfoBase + 0x50),
            bankHandle);

        CCharString::~CCharString(reinterpret_cast<CCharString*>(&local_4));

        CBankFile* const bankFile = *reinterpret_cast<CBankFile**>(&paramSlot);
        if (bankFile != nullptr) {
            auto* const refCounted =
                reinterpret_cast<RefCountedBankFileOverlay*>(bankFile);
            const int oldRefCount = refCounted->refCount;
            refCounted->refCount = oldRefCount - 1;
            if (oldRefCount - 1 == 0) {
                reinterpret_cast<void(__thiscall*)(CBankFile*)>(
                    refCounted->vftable[1])(bankFile);
            }
        }
    }
}