#include <cstddef>
#include <cstdint>

struct CGraphicFrameData;
void __cdecl operator_delete(void*);

struct CGraphicFrameDataReleaseOverlay {
    struct NestedReleaseOverlay {
        std::int32_t referenceCount; // 0x00
        void* releaseCallback_04;    // 0x04
        void* releaseThis_08;        // 0x08
    };

    std::int32_t referenceCount;     // 0x00
    std::byte pad_04[0x04];          // 0x04
    void* field_08;                  // 0x08
    NestedReleaseOverlay* field_0C;  // 0x0C
    std::byte pad_10[0x0A];          // 0x10
    std::uint8_t flags_1A;           // 0x1A
};

static_assert(offsetof(CGraphicFrameDataReleaseOverlay, referenceCount) == 0x00);
static_assert(offsetof(CGraphicFrameDataReleaseOverlay, field_08) == 0x08);
static_assert(offsetof(CGraphicFrameDataReleaseOverlay, field_0C) == 0x0C);
static_assert(offsetof(CGraphicFrameDataReleaseOverlay, flags_1A) == 0x1A);

static_assert(offsetof(CGraphicFrameDataReleaseOverlay::NestedReleaseOverlay, referenceCount) == 0x00);
static_assert(offsetof(CGraphicFrameDataReleaseOverlay::NestedReleaseOverlay, releaseCallback_04) == 0x04);
static_assert(offsetof(CGraphicFrameDataReleaseOverlay::NestedReleaseOverlay, releaseThis_08) == 0x08);

#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) static void InvokeNestedReleaseRaw(void* callback, void* receiver)
{
    __asm {
        mov ecx, [esp + 8]
        mov eax, [esp + 4]
        call eax
        ret 8
    }
}
#else
static void InvokeNestedReleaseRaw(void*, void*)
{
    // Requires an x86 MSVC build to preserve the exact observed
    // `mov ecx, [nested+0x08]; call [nested+0x04]` dispatch shape.
}
#endif

void __fastcall CGraphicFrameData_ReleaseReference(CGraphicFrameData* const param_1)
{
    auto* const self = reinterpret_cast<CGraphicFrameDataReleaseOverlay*>(param_1);

    self->referenceCount = self->referenceCount + -1;
    if (self->referenceCount == 0) {
        if ((self->flags_1A & 0x10) != 0) {
            CGraphicFrameData::Uninitialise(param_1);
        }

        CGraphicFrameDataReleaseOverlay::NestedReleaseOverlay* const nested = self->field_0C;
        if (nested != nullptr) {
            nested->referenceCount = nested->referenceCount + -1;
            if (self->field_0C->referenceCount == 0) {
                InvokeNestedReleaseRaw(self->field_0C->releaseCallback_04, self->field_0C->releaseThis_08);
                operator_delete(self->field_0C);
            }
        }

        self->field_08 = nullptr;
        self->field_0C = nullptr;
        operator_delete(param_1);
    }
}