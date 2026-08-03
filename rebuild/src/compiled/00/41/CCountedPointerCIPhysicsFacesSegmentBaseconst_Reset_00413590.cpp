#include "rebuild_abi.h"

// CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset(
//     CIPhysicsFacesSegmentBase const*) @ 0x00413590.
//
// Genuine C++ reconstruction of the intrusive-refcount smart-pointer Reset
// (real statements only; no inline assembly of any kind). Reset releases the
// currently-held reference then rebinds the smart pointer to a new object:
//
//   * If a control block is held, decrement its owner count. When that count
//     reaches zero, invoke the stored per-object destroy thunk (this in ecx =
//     control->object) and free the control block via the masked deleter.
//   * Unconditionally clear this->control and store the incoming object.
//   * If the incoming object is non-null, allocate a fresh 12-byte control
//     block (masked allocator), seed it with owners=1, the masked destroy-thunk
//     address (0x00413300) and the object, and install it as this->control.
//     A failed allocation leaves this->control cleared.
//
// __fastcall modeled as __fastcall(this in ecx); the single object argument is
// on the stack, so the epilogue pops it (ret 4).
//
// Each statement re-dereferences this->control on purpose: the release block
// reads this->control three separate times (dec, zero-check, deleter arg),
// matching the aliasing-conservative reloads the compiler emits because the
// decrement writes through the pointer. The two this->control = 0 stores must
// NOT be tail-merged: the unconditional pre-store uses an immediate zero while
// the allocation-failure store shares the epilogue via a zeroed register, so
// the failure case is spelled as a distinct else branch.

typedef void(FABLE_FASTCALL *FableFacesDestroyObject_00413590)(void *object);

struct FableFacesControl_00413590
{
    fable_i32 owners;
    FableFacesDestroyObject_00413590 destroyObject;
    void *object;
};

struct FableFacesCountedPointer_00413590
{
    void *object;
    FableFacesControl_00413590 *control;
};

extern void FABLE_FASTCALL FableFacesObjectDelete_00413300(void *object);

extern "C" void *FABLE_CDECL FableFacesControlAllocate_00413590(fable_u32 size);
extern "C" void FABLE_CDECL FableFacesControlDelete_00413590(void *control);

extern "C" void FABLE_FASTCALL FableFacesCountedPointerReset_00413590(
    FableFacesCountedPointer_00413590 *self,
    int edxDummy,
    void *newObject)
{
    (void)edxDummy;

    if (self->control != 0)
    {
        --self->control->owners;
        if (self->control->owners == 0)
        {
            self->control->destroyObject(self->control->object);
            FableFacesControlDelete_00413590(self->control);
        }
    }

    self->control = 0;
    self->object = newObject;

    if (newObject != 0)
    {
        FableFacesControl_00413590 *fresh =
            (FableFacesControl_00413590 *)FableFacesControlAllocate_00413590(0xc);
        FableFacesControl_00413590 *result;
        if (fresh != 0)
        {
            void *o = self->object;
            fresh->owners = 1;
            fresh->destroyObject = &FableFacesObjectDelete_00413300;
            fresh->object = o;
            result = fresh;
        }
        else
        {
            result = 0;
        }
        self->control = result;
    }
}