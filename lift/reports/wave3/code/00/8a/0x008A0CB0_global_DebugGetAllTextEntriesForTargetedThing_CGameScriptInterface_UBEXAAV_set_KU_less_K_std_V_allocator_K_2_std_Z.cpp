#include <cstddef>
#include <cstdint>
#include <set>

class CGameScriptInterface;
class CBankFile;
class CCharString;
class CDataBank;
class CTCBase;
struct LTextGroup;
struct LTextBinNode;

template <typename K, typename V, typename C>
class CVectorMap;

template <typename T>
class CBasicString;

template <typename T>
struct LTextBinTree;

enum ETCInterfaceType : int;

namespace NGameText {
class CDataBank {
public:
    static void GetAllTextEntriesForNarrator(
        ::CDataBank* dataBank,
        CCharString* narrator,
        std::set<unsigned long>& outEntries,
        unsigned long context);
};
}

template <typename K, typename V>
struct CKeyPairCompareLess;

struct BankOwnerOverlay {
    std::byte pad_00[0x14];
    CDataBank* dataBank;
};
static_assert(offsetof(BankOwnerOverlay, dataBank) == 0x14);

struct BasicStringOverlay {
    char* buffer;
};
static_assert(offsetof(BasicStringOverlay, buffer) == 0x0);

struct VectorMapEntryOverlay {
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(VectorMapEntryOverlay, key) == 0x0);
static_assert(offsetof(VectorMapEntryOverlay, value) == 0x4);

struct VectorMapOverlay {
    std::byte pad_00[0x4];
    VectorMapEntryOverlay* end;
};
static_assert(offsetof(VectorMapOverlay, end) == 0x4);

struct TargetedThingOverlay {
    std::byte pad_00[0x20];
    std::uint32_t flags20;
    std::byte pad_24[0x44 - 0x24];
    VectorMapOverlay interfaces;
    std::byte pad_4c[0x91 - 0x4c];
    std::uint8_t flags91;
};
static_assert(offsetof(TargetedThingOverlay, flags20) == 0x20);
static_assert(offsetof(TargetedThingOverlay, interfaces) == 0x44);
static_assert(offsetof(TargetedThingOverlay, flags91) == 0x91);

struct NarratorTargetOverlay {
    std::byte pad_00[0x20];
    std::uint32_t flags20;
    std::byte pad_24[0x44 - 0x24];
    VectorMapOverlay interfaces;
    std::byte pad_4c[0x6c - 0x4c];
    std::uint8_t flags6c;
};
static_assert(offsetof(NarratorTargetOverlay, flags20) == 0x20);
static_assert(offsetof(NarratorTargetOverlay, interfaces) == 0x44);
static_assert(offsetof(NarratorTargetOverlay, flags6c) == 0x6c);

struct LTextTreeRootOverlay {
    std::byte pad_00[0x4];
    LTextBinNode* node_at_04;
    LTextTreeRootOverlay* link_at_08;
    LTextTreeRootOverlay* link_at_0c;
};
static_assert(offsetof(LTextTreeRootOverlay, node_at_04) == 0x4);
static_assert(offsetof(LTextTreeRootOverlay, link_at_08) == 0x8);
static_assert(offsetof(LTextTreeRootOverlay, link_at_0c) == 0xc);

struct LTextTreeWalkThroughOverlay {
    LTextTreeRootOverlay* root;
    void* state_at_04;
};
static_assert(offsetof(LTextTreeWalkThroughOverlay, root) == 0x0);
static_assert(offsetof(LTextTreeWalkThroughOverlay, state_at_04) == 0x4);

template <>
struct LTextBinTree<LTextGroup*> {
    struct LTextTreeWalkThrough {
        static void __thiscall BuildTreeArray(
            LTextTreeWalkThroughOverlay* self,
            LTextBinNode* node);
    };
};

extern BankOwnerOverlay* DAT_013b86a0;
extern unsigned long DAT_013b86c0;
extern CCharString DAT_013b86f8;
extern const char DAT_0122d70e[];
extern const char DAT_012783c8[];

extern std::uint64_t Catch_0040d959();
extern char CCharString__NotEqual(CCharString* lhs, const char* rhs);
extern CCharString* CCharString__AppendCString(CCharString* lhs, const char* rhs);

template <typename K, typename V, typename C>
extern VectorMapEntryOverlay* __thiscall LowerBoundThunk(
    CVectorMap<K, V, C>* self,
    const K* key);

class CBankFile {
public:
    CCharString* GetBankHandle();
};

void CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing(
    std::set<unsigned long>& outEntries) const
{
    auto* const walk =
        reinterpret_cast<LTextTreeWalkThroughOverlay*>(&outEntries);

    if (walk->state_at_04 != nullptr) {
        LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray(
            walk,
            walk->root->node_at_04);
        walk->root->link_at_08 = walk->root;
        walk->root->node_at_04 = nullptr;
        walk->root->link_at_0c = walk->root;
        walk->state_at_04 = nullptr;
    }

    bool useTargetedThing;
    if (reinterpret_cast<BasicStringOverlay*>(&DAT_013b86f8)->buffer == nullptr) {
        int remaining = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";
        do {
            if (remaining == 0) {
                break;
            }
            --remaining;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);
        useTargetedThing = equal;
    } else {
        useTargetedThing =
            CBasicString<char>::operator==(
                reinterpret_cast<CBasicString<char>*>(&DAT_013b86f8),
                "");
    }

    if (useTargetedThing) {
        const std::uint64_t catchResult = Catch_0040d959();
        auto* const targetedThing =
            reinterpret_cast<TargetedThingOverlay*>(
                static_cast<std::uintptr_t>(
                    static_cast<std::uint32_t>(catchResult)));

        if (targetedThing != nullptr &&
            (targetedThing->flags91 & 0x01) == 0 &&
            (targetedThing->flags20 & 0x100) != 0) {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(8);
            auto* interfaceEntry =
                LowerBoundThunk<
                    ETCInterfaceType,
                    CTCBase*,
                    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>(
                    reinterpret_cast<CVectorMap<
                        ETCInterfaceType,
                        CTCBase*,
                        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
                        &targetedThing->interfaces),
                    &interfaceType);

            if (interfaceEntry == targetedThing->interfaces.end ||
                interfaceType < interfaceEntry->key) {
                interfaceEntry = targetedThing->interfaces.end;
            }

            auto* const interfaceObject = interfaceEntry->value;
            using RawSlot84Fn = int(__thiscall*)(void*);
            auto* const vftable = *reinterpret_cast<void***>(interfaceObject);
            auto* const slot84 =
                reinterpret_cast<RawSlot84Fn>(vftable[0x84 / sizeof(void*)]);
            auto* const narratorTarget =
                reinterpret_cast<NarratorTargetOverlay*>(slot84(interfaceObject));

            if (narratorTarget != nullptr &&
                (narratorTarget->flags6c & 0x10) != 0) {
                if (static_cast<std::int8_t>(narratorTarget->flags20 >> 8) >= 0) {
                    return;
                }

                interfaceType = static_cast<ETCInterfaceType>(0x0f);
                auto* bankEntry =
                    LowerBoundThunk<
                        ETCInterfaceType,
                        CTCBase*,
                        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>(
                        reinterpret_cast<CVectorMap<
                            ETCInterfaceType,
                            CTCBase*,
                            CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
                            &narratorTarget->interfaces),
                        &interfaceType);

                if (bankEntry == narratorTarget->interfaces.end ||
                    interfaceType < bankEntry->key) {
                    bankEntry = narratorTarget->interfaces.end;
                }

                auto* const bankFile = reinterpret_cast<CBankFile*>(bankEntry->value);
                CCharString* const bankHandleForCompare = bankFile->GetBankHandle();
                if (CCharString__NotEqual(bankHandleForCompare, DAT_0122d70e) == '\0') {
                    return;
                }

                CCharString* const bankHandleForNarrator = bankFile->GetBankHandle();
                NGameText::CDataBank::GetAllTextEntriesForNarrator(
                    DAT_013b86a0->dataBank,
                    bankHandleForNarrator,
                    outEntries,
                    DAT_013b86c0);

                if (walk->state_at_04 != nullptr) {
                    return;
                }

                CCharString* const suffixedBankHandle =
                    CCharString__AppendCString(bankFile->GetBankHandle(), DAT_012783c8);

                NGameText::CDataBank::GetAllTextEntriesForNarrator(
                    DAT_013b86a0->dataBank,
                    suffixedBankHandle,
                    outEntries,
                    DAT_013b86c0);

                return;
            }
        }
    }

    NGameText::CDataBank::GetAllTextEntriesForNarrator(
        DAT_013b86a0->dataBank,
        reinterpret_cast<CCharString*>(&DAT_013b86f8),
        outEntries,
        DAT_013b86c0);
}