#include "fable_string.h"
#include "fable_persist_context.h"

template<> void CPersistContext_TransferableComponent<unsigned char>::TransferOut(CPersistContext& context, CCharString& text)
{
    long value = *TransVal;
    CPersistTraitsSLONG::TransferOut(context, text, value);
}
