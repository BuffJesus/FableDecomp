#include "fable_def_string.h"

CDefString::operator CCharString()
{
    return GDefStringTable.GetString(TablePos);
}
