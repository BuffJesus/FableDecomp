#include "fable_string.h"
CCharString::operator const char*() const {
 if(storage_==0)return g_FableEmptyNarrowString_0129AAF4;
 return storage_->text;
}
