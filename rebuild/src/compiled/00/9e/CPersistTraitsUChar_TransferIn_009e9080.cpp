// CPersistTraits<unsigned char>::TransferIn @ 0x009E9080
// (Ego_r ?TransferIn@?$CPersistTraits@E@@QAEXAAVCPersistContext@@AAVCStringParser@@AAE@Z).
// Reads the token as an SLONG through the static converter and keeps the low byte.
#include "fable_string.h"
#include "fable_persist_context.h"

void CPersistTraits<unsigned char>::TransferIn(CPersistContext& context, CStringParser& parser, unsigned char& value)
{
    long converted;
    CPersistTraitsSLONG::TransferIn(context, parser, converted);
    value = static_cast<unsigned char>(converted);
}
