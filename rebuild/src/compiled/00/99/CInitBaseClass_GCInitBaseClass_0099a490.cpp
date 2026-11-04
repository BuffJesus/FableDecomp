#include "fable_init_base_class.h"

// CBaseClass's destructor and CInitBaseClass::Uninitialise share this translation
// unit and inline into the destructor.
inline CBaseClass::~CBaseClass()
{
}

void CInitBaseClass::Uninitialise()
{
    if (Valid == true)
    {
        --g_CInitBaseClassValid_013BCA14;
        Valid = false;
    }
}

CInitBaseClass::~CInitBaseClass()
{
    Uninitialise();
}