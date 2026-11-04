#include "test_support/compressor_virtual_stubs.h"
#include <stdio.h>
#include <string.h>
#include <new>
static int calls,bad;static unsigned char storage[32];static unsigned long wanted;
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
namespace _STL {template<> void vector<unsigned char>::resize(size_t n,const unsigned char& value){
 ++calls;if(n!=wanted || value || _M_start || _M_finish || _M_end_of_storage._M_data)bad=1;
 _M_start=n?storage:0;_M_finish=n?storage+n:0;_M_end_of_storage._M_data=_M_finish;
}}
int main(){
 double raw[sizeof(CACompressor)/8+1];
 for(int i=0;i<2;++i){memset(raw,0xa5,sizeof(raw));wanted=i?13:0;
 CACompressor* c=new(raw) CACompressor(wanted);
 if(c->Workspace!=(wanted?storage:0) || c->WorkspaceSize!=wanted || c->OwnWorkspace.size()!=wanted || reinterpret_cast<unsigned char*>(c)[12]!=0xa5)return 1;
 }
 if(calls!=2 || bad)return 2;puts("BASE_CTOR_PASS");return 0;
}