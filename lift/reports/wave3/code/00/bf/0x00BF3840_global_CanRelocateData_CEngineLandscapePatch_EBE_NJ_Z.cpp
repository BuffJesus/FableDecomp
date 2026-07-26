#include <cstddef>

struct CEngineLandscapePatch_CanRelocateData_InnerOverlay {
    std::byte pad_0000[0x5C];
    char field_005C;
};

static_assert(offsetof(CEngineLandscapePatch_CanRelocateData_InnerOverlay, field_005C) == 0x5C);

struct CEngineLandscapePatch_CanRelocateData_Overlay {
    std::byte pad_0000[0x04];
    CEngineLandscapePatch_CanRelocateData_InnerOverlay* field_0004;
};

static_assert(offsetof(CEngineLandscapePatch_CanRelocateData_Overlay, field_0004) == 0x04);

bool CEngineLandscapePatch::CanRelocateData(long /*unused*/) const
{
    auto const* self = reinterpret_cast<CEngineLandscapePatch_CanRelocateData_Overlay const*>(this);
    return self->field_0004->field_005C == '\0';
}