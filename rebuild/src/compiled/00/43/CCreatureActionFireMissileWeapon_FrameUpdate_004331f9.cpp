// CPersistContext's binary-only vector transfer, <CCharString> @ 0x004331F9
// (see TransferBinaryVector in fable_persist_context.h; CFrontEndDef::Transfer
// 0x004331C0 calls it for vAttractModeMovie): CheckCRC(""), then
// GFSerialiseVectorBinaryIn 0x00433273 when loading, otherwise
// GFSerialiseVectorBinaryOut 0x00433227. Size-optimised unit (/Os).
#pragma optimize("s", on)
#include "fable_persist_context.h"
#include "fable_string.h"

template void CPersistContext::TransferBinaryVector<CCharString>(_STL::vector<CCharString>& value);
