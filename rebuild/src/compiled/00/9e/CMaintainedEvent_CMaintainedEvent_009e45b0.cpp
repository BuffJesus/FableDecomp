// CMaintainedEvent (Ego_r class, 0x38 bytes: CBaseClass + CInputEvent Event),
// the members retail's device-input unit (lib_joystick, 0x009E41A0-0x009E4A95)
// emits for it, in retail order:
//   0x009E45B0 ??0CMaintainedEvent@@QAE@XZ
//   0x009E45D0 ??1CMaintainedEvent@@UAE@XZ  (implicit; Ego_d/FableWin list it
//              right after the ctor; no direct caller, the temporaries inline it)
//   0x009E45F0 ??_GCMaintainedEvent@@UAEPAXI@Z (slot 0 of vtable 0x0129C0E0,
//              RTTI .?AVCMaintainedEvent@@; ??_E aliases it)
// The out-of-line implicit destructor comes from STLport's
// list<CMaintainedEvent>::insert(iterator) (retail 0x009E4A20), which inserts a
// value-initialised temporary: CJoystick::ProcessMaintainedEvents 0x009E4470
// appends a default event that way before filling it in.
#include "fable_input_devices.h"

CMaintainedEvent::CMaintainedEvent()
{
}

namespace _STL {
template<class T, class A>
typename list<T, A>::iterator list<T, A>::insert(iterator position)
{
    return insert(position, T());
}
template list<CMaintainedEvent>::iterator list<CMaintainedEvent>::insert(list<CMaintainedEvent>::iterator);
}
