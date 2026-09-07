#include <cstddef>
#include <cstdint>
#include <new>

struct CThing;
template <typename T>
class CIntelligentPointer;

using CThingConstPtr = CIntelligentPointer<const CThing>;

struct CQuickAccessItemButtonGuiDef {
    std::byte raw[0x50];
};
static_assert(sizeof(CQuickAccessItemButtonGuiDef) == 0x50);

struct CQuickAccessItemButtonGuiDefVectorOverlay {
    CQuickAccessItemButtonGuiDef* first;
    CQuickAccessItemButtonGuiDef* last;
    CQuickAccessItemButtonGuiDef* end;
};
static_assert(offsetof(CQuickAccessItemButtonGuiDefVectorOverlay, first) == 0x0);
static_assert(offsetof(CQuickAccessItemButtonGuiDefVectorOverlay, last) == 0x4);
static_assert(offsetof(CQuickAccessItemButtonGuiDefVectorOverlay, end) == 0x8);
static_assert(sizeof(CQuickAccessItemButtonGuiDefVectorOverlay) == 0x0C);

struct CActiveFile;

namespace CFileInstaller {
struct CActiveFile {
    static void __thiscall OnReadFinished(::CActiveFile* self);
};
}

extern CQuickAccessItemButtonGuiDef* __cdecl CQuickAccessItemButtonGuiDef_CopyConstructArray(
    std::uint32_t count,
    CQuickAccessItemButtonGuiDef* srcFirst,
    CQuickAccessItemButtonGuiDef* srcLast);

extern void* __cdecl Vector_CopyElements(
    CQuickAccessItemButtonGuiDef* dstFirst,
    CThingConstPtr* tempBegin);

namespace std {
template <>
class vector<CThingConstPtr, allocator<CThingConstPtr>> {
public:
    static void __thiscall _Destroy(
        void* destroyThis,
        CThingConstPtr* tempBegin,
        CThingConstPtr* destroyEnd);
};
}

CActiveFile* __thiscall CActiveFile_AssignQuickAccessItems(
    CActiveFile* self,
    CQuickAccessItemButtonGuiDefVectorOverlay* other)
{
    auto* const selfVec = reinterpret_cast<CQuickAccessItemButtonGuiDefVectorOverlay*>(self);
    CThingConstPtr* unaff_EDI;
    CQuickAccessItemButtonGuiDefVectorOverlay* const sourceVec = other;

    if (other != reinterpret_cast<CQuickAccessItemButtonGuiDefVectorOverlay*>(self)) {
        const std::uint32_t otherCount =
            static_cast<std::uint32_t>((sourceVec->last - sourceVec->first));

        if (static_cast<std::uint32_t>(selfVec->end - selfVec->first) < otherCount) {
            CQuickAccessItemButtonGuiDef* const newFirst =
                CQuickAccessItemButtonGuiDef_CopyConstructArray(
                    otherCount,
                    sourceVec->first,
                    sourceVec->last);

            CFileInstaller::CActiveFile::OnReadFinished(self);
            selfVec->first = newFirst;
            selfVec->end = reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                reinterpret_cast<std::uintptr_t>(newFirst) + otherCount * 0x50);
        } else {
            other = reinterpret_cast<CQuickAccessItemButtonGuiDefVectorOverlay*>(
                reinterpret_cast<std::uintptr_t>(other) & 0x00FFFFFF);

            auto* const tempBegin =
                reinterpret_cast<CThingConstPtr*>(reinterpret_cast<std::byte*>(&other) + 3);
            *reinterpret_cast<std::byte*>(tempBegin) = std::byte{0};

            if (static_cast<std::uint32_t>(selfVec->last - selfVec->first) < otherCount) {
                Vector_CopyElements(selfVec->first, tempBegin);

                CQuickAccessItemButtonGuiDef* src =
                    reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                        static_cast<std::uint32_t>(selfVec->last - selfVec->first) * 0x50 +
                        reinterpret_cast<std::uintptr_t>(sourceVec->first));
                CQuickAccessItemButtonGuiDef* dst = selfVec->last;
                CQuickAccessItemButtonGuiDef* const srcEnd = sourceVec->last;

                for (; src != srcEnd;
                     src = reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                         reinterpret_cast<std::uintptr_t>(src) + 0x50)) {
                    if (dst != nullptr) {
                        ::new (dst) CQuickAccessItemButtonGuiDef(*src);
                    }

                    dst = reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
                        reinterpret_cast<std::uintptr_t>(dst) + 0x50);
                }
            } else {
                void* const destroyThis = Vector_CopyElements(selfVec->first, tempBegin);
                std::vector<CThingConstPtr, std::allocator<CThingConstPtr>>::_Destroy(
                    destroyThis,
                    tempBegin,
                    unaff_EDI);
            }
        }

        selfVec->last = reinterpret_cast<CQuickAccessItemButtonGuiDef*>(
            otherCount * 0x50 + reinterpret_cast<std::uintptr_t>(selfVec->first));
    }

    return self;
}