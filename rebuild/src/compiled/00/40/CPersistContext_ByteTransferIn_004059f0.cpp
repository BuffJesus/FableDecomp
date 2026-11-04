#include "fable_string.h"
#include "fable_persist_context.h"

template<> void CPersistContext_TransferableComponent<unsigned char>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    *TransVal = static_cast<unsigned char>(value);
}
