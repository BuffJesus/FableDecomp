// CPersistContext's binary-only member transfer, <long> @ 0x00431102 (see
// TransferBinaryMember in fable_persist_context.h; CFrontEndDef::Transfer
// 0x004331C0 calls it for its graphic indices): CheckCRC(""), then
// CPersistTraits<long>::TransferBinaryIn 0x0040FE60 when loading or
// CDataOutputStream::WriteSLONG 0x00993EB0 when saving. Size-optimised unit
// (/Os: the ebp frame homes the traits object).
#pragma optimize("s", on)
#include "fable_persist_context.h"

template void CPersistContext::TransferBinaryMember<long>(long& value);
