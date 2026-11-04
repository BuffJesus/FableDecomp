// CBasicString<char>::operator<(const CBasicString<char>&) const @ 0x00429950
// (FableWin ??M?$CBasicString@D@@QBE_NABV0@@Z): the text order, Compare < 0.
// Out of line where a caller does not expand it (map<CCharString, long>::
// operator[] 0x00519230); this copy expands Compare (0x00411570).
// Replaces a machine-code bake of the same bytes.
#include "fable_basic_string.h"

template bool CBasicString<char>::operator<(const CBasicString<char>& other) const;
