// lib_input_event.cpp, retail 0x00A043D0 / 0x00A043F0 (Ego_r
// CInputEvent::GetMouseMovement / GetHeldMouseMovement): the movement vectors
// by value. Mouse-button held events keep theirs in the first data union.
// Near matches, not gated: retail copies a C3DVector member dword by dword at
// fixed offsets. C3DVector has user-declared constructors, so VC7.1 cannot put
// it in the data unions (C2620); the float-storage forms here load through FPU.
#define FABLE_INPUT_EVENT_MOVEMENT_OUT_OF_LINE
#include "fable_input_event.h"

// Retail 00A043D0.
C3DVector CInputEvent::GetMouseMovement() const
{
    return C3DVector(MouseMovementStorage_[0], MouseMovementStorage_[1], MouseMovementStorage_[2]);
}

// Retail 00A043F0.
C3DVector CInputEvent::GetHeldMouseMovement() const
{
    return C3DVector(HeldMouseMovementStorage_[0], HeldMouseMovementStorage_[1], HeldMouseMovementStorage_[2]);
}
