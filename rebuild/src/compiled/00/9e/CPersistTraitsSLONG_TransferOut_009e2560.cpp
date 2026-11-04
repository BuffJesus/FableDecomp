#include "fable_string.h"
#include "fable_persist_context.h"

void FABLE_FASTCALL CPersistTraitsSLONG::TransferOut(CPersistContext&, CCharString& text, const long& value)
{
    text += GFIntToCharString(value);
}
