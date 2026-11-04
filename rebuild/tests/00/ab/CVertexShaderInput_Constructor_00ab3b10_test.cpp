#include "fable_vertex_shader_input.h"
#include <stdio.h>
#include <string.h>
#include <new>
#include <stdlib.h>
static unsigned calls,failed,mutation;
static void* expected;
CBaseClass::CBaseClass(){if(this!=expected)++failed;++calls;if(mutation)memset(this,0x73,28);}
CBaseClass::~CBaseClass(){abort();}
extern "C" void UnexpectedVertexInputDeletion(){abort();}
#pragma comment(linker,"/alternatename:??1CVertexShaderInput@@UAE@XZ=_UnexpectedVertexInputDeletion")
int main(){
 unsigned cases=0;
 for(unsigned fill=0;fill<256;++fill)for(mutation=0;mutation<2;++mutation){
  unsigned long storage[9];memset(storage,fill,sizeof(storage));expected=storage+1;calls=0;
  CVertexShaderInput* p=new(expected) CVertexShaderInput;
  if(p!=expected||calls!=1||!storage[1])++failed;
  for(unsigned i=2;i<8;++i)if(storage[i])++failed;
  const unsigned long sentinel=fill*0x01010101UL;
  if(storage[0]!=sentinel||storage[8]!=sentinel)++failed;
  ++cases;
 }
 if(failed)return 1;
 printf("VERTEX_INPUT_CONSTRUCTOR_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
