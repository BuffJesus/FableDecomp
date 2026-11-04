#include "test_support/checksum_string_fixture.h"
#include "../src/compiled/00/9a/CBasicString_char_AppendRepeat_009a0460.cpp"
int main(){
 {CCharString empty;require(&(empty+='\0')==&empty && dataOf(empty)==0 && !copyCalls && !extendCalls);
  require(&(empty+='a')==&empty && dataOf(empty)->Data.StringLength==1 && !copyCalls && !extendCalls);
  CCharString shared(empty);require(dataOf(shared)==dataOf(empty) && dataOf(empty)->owners==2);
  empty+=static_cast<char>(0x80);require(copyCalls==1 && extendCalls==1 && dataOf(shared)->owners==1 && dataOf(empty)!=dataOf(shared));
  require(dataOf(empty)->Data.StringLength==2 && static_cast<unsigned char>(dataOf(empty)->text[1])==0x80 && !strcmp(dataOf(shared)->text,"a"));
  empty+='\0';require(dataOf(empty)->Data.StringLength==3 && dataOf(empty)->text[2]==0 && dataOf(empty)->text[3]==0);
  empty+='z';require(dataOf(empty)->Data.StringLength==4 && dataOf(empty)->text[3]=='z' && dataOf(empty)->text[4]==0);
 }
 require(!liveData);puts("CHAR_STRING_APPEND_CHAR_0099f180_PASS");return 0;
}
