#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CWideString {
    std::uint32_t _opaque;

    CWideString& operator=(const CWideString& rhs);
};

struct CCharString {
    std::uint32_t _opaque;

    ~CCharString();
};

static_assert(sizeof(CWideString) == 4);
static_assert(sizeof(CCharString) == 4);

struct ArrayOverlay {
    CWideString* begin;       // 0x00
    CWideString* end;         // 0x04
    CWideString* capacityEnd; // 0x08
};

static_assert(offsetof(ArrayOverlay, begin) == 0x00);
static_assert(offsetof(ArrayOverlay, end) == 0x04);
static_assert(offsetof(ArrayOverlay, capacityEnd) == 0x08);

static void __thiscall CWideString_CopyConstruct(CWideString* self, const CWideString* src);
static CWideString* __cdecl _Uninit_copy_CWideString(
    const CWideString* first,
    const CWideString* last,
    CWideString* dest,
    void* context);

ArrayOverlay* __thiscall Array_AssignFrom(ArrayOverlay* self, ArrayOverlay* other)
{
    if (other != self) {
        CWideString* dstBegin = self->begin;
        CWideString* srcEnd = other->end;
        CWideString* srcBegin = other->begin;
        const std::uint32_t srcCount =
            static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(srcEnd) -
                 reinterpret_cast<std::uintptr_t>(srcBegin)) >> 2);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(self->capacityEnd) -
                 reinterpret_cast<std::uintptr_t>(dstBegin)) >> 2) < srcCount) {
            void* newStorage;
            if (srcCount == 0) {
                newStorage = nullptr;
            } else {
                newStorage = std::malloc(srcCount * 4);
            }

            if (srcBegin != srcEnd) {
                const std::intptr_t delta =
                    reinterpret_cast<std::intptr_t>(newStorage) -
                    reinterpret_cast<std::intptr_t>(srcBegin);
                do {
                    CWideString* const newElem =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::intptr_t>(srcBegin) + delta);
                    if (newElem != nullptr) {
                        CWideString_CopyConstruct(newElem, srcBegin);
                    }
                    srcBegin =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::uintptr_t>(srcBegin) + 4);
                } while (srcBegin != srcEnd);
            }

            CWideString* oldEnd = self->end;
            for (CWideString* it = self->begin; it != oldEnd;
                 it = reinterpret_cast<CWideString*>(
                     reinterpret_cast<std::uintptr_t>(it) + 4)) {
                reinterpret_cast<CCharString*>(it)->~CCharString();
            }

            if (self->begin != nullptr) {
                std::free(self->begin);
            }

            self->begin = static_cast<CWideString*>(newStorage);
            self->capacityEnd =
                reinterpret_cast<CWideString*>(
                    reinterpret_cast<std::uintptr_t>(newStorage) + srcCount * 4);
            self->end =
                reinterpret_cast<CWideString*>(
                    reinterpret_cast<std::uintptr_t>(self->begin) + srcCount * 4);
            return self;
        }

        const std::uint32_t dstCount =
            static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(self->end) -
                 reinterpret_cast<std::uintptr_t>(dstBegin)) >> 2);

        if (dstCount < srcCount) {
            int remaining = static_cast<int>((dstCount * 4) >> 2);
            if (0 < remaining) {
                do {
                    dstBegin->operator=(*srcBegin);
                    srcBegin =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::uintptr_t>(srcBegin) + 4);
                    dstBegin =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::uintptr_t>(dstBegin) + 4);
                    remaining = remaining + -1;
                } while (remaining != 0);
            }

            _Uninit_copy_CWideString(
                reinterpret_cast<CWideString*>(
                    reinterpret_cast<std::uintptr_t>(other->begin) +
                    (((reinterpret_cast<std::uintptr_t>(self->end) -
                       reinterpret_cast<std::uintptr_t>(self->begin)) >> 2) * 4)),
                other->end,
                self->end,
                &other);
        } else {
            int remaining = static_cast<int>(
                (reinterpret_cast<std::uintptr_t>(srcEnd) -
                 reinterpret_cast<std::uintptr_t>(srcBegin)) >> 2);
            if (0 < remaining) {
                do {
                    dstBegin->operator=(*srcBegin);
                    srcBegin =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::uintptr_t>(srcBegin) + 4);
                    dstBegin =
                        reinterpret_cast<CWideString*>(
                            reinterpret_cast<std::uintptr_t>(dstBegin) + 4);
                    remaining = remaining + -1;
                } while (remaining != 0);
            }

            CWideString* oldEnd = self->end;
            for (; dstBegin != oldEnd;
                 dstBegin = reinterpret_cast<CWideString*>(
                     reinterpret_cast<std::uintptr_t>(dstBegin) + 4)) {
                reinterpret_cast<CCharString*>(dstBegin)->~CCharString();
            }
        }

        self->end =
            reinterpret_cast<CWideString*>(
                reinterpret_cast<std::uintptr_t>(self->begin) + srcCount * 4);
    }

    return self;
}