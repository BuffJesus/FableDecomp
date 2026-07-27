#include <cstddef>
#include <cstdint>

struct InputEvent_CheckKeyAndGetFrames_Overlay
{
    std::byte pad_00[0x80];
    std::byte subobject_80;
};

static_assert(offsetof(InputEvent_CheckKeyAndGetFrames_Overlay, subobject_80) == 0x80);

void __thiscall InputEvent_CheckKeyAndGetFrames(void* thisPtr, EInputKey key)
{
    auto* const overlay = static_cast<InputEvent_CheckKeyAndGetFrames_Overlay*>(thisPtr);
    auto* const inputEvent = reinterpret_cast<CInputEvent*>(&overlay->subobject_80);

    const EInputKey currentKey = CInputEvent::GetKey(inputEvent);
    if (key < currentKey)
    {
        CCreatureActionBase::GetFramesRemaining(
            reinterpret_cast<CCreatureActionBase*>(&overlay->subobject_80));
    }
}