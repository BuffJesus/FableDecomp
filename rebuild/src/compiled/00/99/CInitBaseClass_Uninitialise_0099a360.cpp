#include "fable_init_base_class.h"

void CInitBaseClass::Uninitialise()
{
    if (Valid == true)
    {
        --g_CInitBaseClassValid_013BCA14;
        Valid = false;
    }
}
