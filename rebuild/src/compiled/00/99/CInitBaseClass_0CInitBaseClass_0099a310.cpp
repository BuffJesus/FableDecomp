#include "fable_init_base_class.h"

// CBaseClass's constructor shares this translation unit and inlines here; its
// vtable store is dead once CInitBaseClass installs its own.
inline CBaseClass::CBaseClass()
{
}

CInitBaseClass::CInitBaseClass()
    : Valid(false)
{
    ++g_CInitBaseClassConstructed_013BCA18;
}