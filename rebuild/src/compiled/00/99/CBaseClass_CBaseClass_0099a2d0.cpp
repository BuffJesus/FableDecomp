#include "fable_base_class.h"

// One retail translation unit (0x0099A2D0..0x0099A470): CBaseClass and
// CBaseClassNonCopyable. Each class's constructor and destructor are emitted out of
// line and also inlined into the other class's bodies here, and
// CBaseClassNonCopyable's vtable (0x0129A7C4) and its scalar deleting destructor
// (0x0099A470, which inlines both destructors) are emitted with them.

// 0x0099A2D0
inline CBaseClass::CBaseClass()
{
}

// 0x0099A2E0
inline CBaseClass::~CBaseClass()
{
}

// 0x0099A2F0
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

// 0x0099A300
CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}
