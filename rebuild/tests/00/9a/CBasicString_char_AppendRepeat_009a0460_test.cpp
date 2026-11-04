#include "test_support/checksum_string_fixture.h"
int main(){
 unsigned lengths[5]={0,1,2,7,19};unsigned chars[3]={0,0x41,0xff};
 for(unsigned n=0;n<5;++n)for(unsigned k=0;k<3;++k){
  CBasicString<char> value("pre");unsigned before=extendCalls;value.Append(static_cast<char>(chars[k]),lengths[n]);
  require(extendCalls==before+1 && value.StringLength==3+lengths[n] && !memcmp(value.PData,"pre",3));
  for(unsigned i=0;i<lengths[n];++i)require(static_cast<unsigned char>(value.PData[3+i])==chars[k]);
  for(unsigned j=value.StringLength;j<((value.StringLength+4)&~3);++j)require(value.PData[j]==0);
  free(value.PData);--liveData;
 }
 require(!liveData);puts("BASIC_STRING_APPEND_REPEAT_009a0460_PASS");return 0;
}
