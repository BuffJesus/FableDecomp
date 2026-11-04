#include "test_support/string_vector_fixture.h"
int main(){
 unsigned long slots[5];for(int i=0;i<5;++i)slots[i]=0xC0DEC0DE;CCharString* first=(CCharString*)(slots+1);
 {CCharString value("repeat");int before=copies;
  if(_STL::uninitialized_fill_n((CCharString*)0,0,value)!=0||copies!=before)return 1;
  if(_STL::uninitialized_fill_n(first,0,value)!=first||slots[1]!=0xC0DEC0DE)return 2;
  if(_STL::uninitialized_fill_n(first,3,value)!=first+3||copies!=before+3||live!=4)return 3;
  if(slots[0]!=0xC0DEC0DE||slots[4]!=0xC0DEC0DE)return 4;
  for(i=0;i<3;++i){if(strcmp(first[i].GetConstCharPtr(),"repeat"))return 5;first[i].~CCharString();}
 }
 if(live||dataLive)return 6;puts("STRING_UNINIT_FILL_PASS");return 0;
}
