#include "fable_string.h"
long CCharString::FindCharPos(char character) const {
 const char* found=strchr(GetConstCharPtr(),character);
 return found ? found-GetConstCharPtr() : -1;
}
