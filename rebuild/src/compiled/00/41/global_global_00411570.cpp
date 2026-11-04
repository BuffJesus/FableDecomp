// CBasicString<char>::Compare(const char*, const char*) @ 0x00411570
// (FableWin ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static __fastcall):
// signed strcmp. Out of line where CBasicString<char>::operator< is expanded
// (map<CCharString, long>::_M_lower_bound 0x0047547D).
#include "fable_basic_string.h"

template long __fastcall CBasicString<char>::Compare(const char* left, const char* right);
