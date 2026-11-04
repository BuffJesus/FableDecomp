#include "fable_persist_context.h"
#include "fable_string.h"

template<> void CPersistContext_TransferableComponent<bool>::TransferOut(CPersistContext& context, CCharString& text)
{
    if (*TransVal)
        text += "TRUE";
    else
        text += "FALSE";
}
