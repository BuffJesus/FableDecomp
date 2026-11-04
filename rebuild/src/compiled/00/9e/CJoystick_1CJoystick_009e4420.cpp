// CJoystick::~CJoystick (retail 0x009E4420) and the scalar deleting destructor
// the CJoystick vtable 0x0129C0EC slot 9 points at (retail 0x009E4980; Ego_r
// names the slot ??_ECJoystick@@UAEPAXI@Z, which VC7.1 aliases to ??_G when no
// array delete is seen). Retail does not inline the destructor into ??_G.
// STLport's list destructor (clear, then release the sentinel node through the
// node allocator's null-checked free) is expanded in ~CJoystick; its own
// out-of-line copies are 0x009E4830/0x009E48F0.
#include "fable_input_devices.h"

namespace _STL {
template<class T, class A>
list<T, A>::~list()
{
    clear();
    if (_M_node)
        free(_M_node);
}
}

CJoystick::~CJoystick()
{
}
