#include "fable_string.h"
bool CCharString::FastCompare(const CCharString& other) const {
 if(storage_==other.storage_)return false;
 if(!storage_)return true;
 if(!other.storage_)return false;
 unsigned long length=storage_->Data.StringLength;
 if(length<other.storage_->Data.StringLength)return true;
 if(length>other.storage_->Data.StringLength)return false;
 const unsigned long* left=(const unsigned long*)storage_->Data.PData;
 const unsigned long* right=(const unsigned long*)other.storage_->Data.PData;
 for(unsigned long i=0;i<length;i+=4,++left,++right){
  if(*left<*right)return true;
  if(*left!=*right)return false;
 }
 return false;
}
