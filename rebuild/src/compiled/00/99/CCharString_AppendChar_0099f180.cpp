#include "fable_string.h"
const CCharString& CCharString::operator+=(char character){
 if(storage_){MakeOwnCopyOfData();storage_->Data.Append(character,1);return *this;}
 const char text[2]={character,0};
 if(text[0])storage_=new CCharStringData(text);
 return *this;
}
