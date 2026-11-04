#include "fable_string_parser.h"
#include "fable_persist_context.h"

template<> void CPersistContext_TransferableComponent<bool>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<bool> traits;
    traits.TransferIn(context, parser, *TransVal);
}
