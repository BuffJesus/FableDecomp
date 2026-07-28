#pragma pack(push, 1)
struct CEngineSceneGrid_Initialize_ThisOverlay {
    void* field_0;
};
static_assert(offsetof(CEngineSceneGrid_Initialize_ThisOverlay, field_0) == 0);

struct CEngineSceneGrid_Initialize_Field0Overlay {
    C3DBoundingBox* field_0;
};
static_assert(offsetof(CEngineSceneGrid_Initialize_Field0Overlay, field_0) == 0);
#pragma pack(pop)

// Observed as a direct call with one stack argument.
extern "C" void __cdecl std__list__list(void* storage);

CEngineSceneGrid* __thiscall CEngineSceneGrid_Initialize(
    CEngineSceneGrid* self,
    unsigned long** param_2)
{
    std__list__list(reinterpret_cast<unsigned char*>(&param_2) + 3);

    const auto* const thisOverlay =
        reinterpret_cast<const CEngineSceneGrid_Initialize_ThisOverlay*>(self);
    const auto* const field0Overlay =
        reinterpret_cast<const CEngineSceneGrid_Initialize_Field0Overlay*>(thisOverlay->field_0);

    self->GetCellBoundingBox(
        field0Overlay->field_0,
        **param_2,
        *param_2);

    return self;
}