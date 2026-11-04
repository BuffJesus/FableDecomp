// GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream&, vector<CCharString>&)
// @ 0x00433227 (Ego_r ??$GFSerialiseVectorBinaryOut@VCCharString@@@@YIX...@Z,
// fastcall): the count, then each string null-terminated. Size-optimised
// unit (/Os: the ebp frame homes the stream and the traits object).
#pragma optimize("s", on)
#include "fable_persist_context.h"
#include "fable_string.h"

template void FABLE_FASTCALL GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream& stream,
                                                                     _STL::vector<CCharString>& value);
