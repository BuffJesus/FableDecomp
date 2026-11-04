#include "test_support/compressor_test_access.h"
#include <stdio.h>
#include <string.h>
static int calls;
void* __cdecl CCompressorZlib::Alloc(void*,unsigned int,unsigned int){++calls;return 0;}
void __cdecl CCompressorZlib::Free(void*,void*){++calls;}
int main(){
 double raw[sizeof(CCompressorZlib)/8];memset(raw,0xa5,sizeof(raw));
 CCompressorZlib* c=reinterpret_cast<CCompressorZlib*>(raw);CCompressorZlibTestAccess::Initialise(*c);
 if(c->Stream.next_in || c->Stream.next_out || c->Stream.avail_in || c->Stream.avail_out || c->ZlibInitialised || c->Compressing || c->ErrorFlag)return 1;
 if(c->Stream.total_in!=0xa5a5a5a5UL || c->Stream.total_out!=0xa5a5a5a5UL || c->WorkspaceSize!=0xa5a5a5a5UL)return 2;
 c->Stream.zalloc(0,1,2);c->Stream.zfree(0,0);if(calls!=2)return 3;
 puts("INIT_PASS");return 0;
}
