#include "fable_data_input_stream.h"
#include <stdio.h>
#include <string.h>
static int baseCalls,rootCalls,bad;static const void* expected;
CBaseClass::~CBaseClass(){++rootCalls;if(this!=expected)bad=1;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){++baseCalls;if(this!=expected)bad=2;}
int main(){
 double raw[4];memset(raw,0x5a,sizeof(raw));CMemoryDataInputStream* stream=reinterpret_cast<CMemoryDataInputStream*>(raw);expected=stream;
 stream->CMemoryDataInputStream::~CMemoryDataInputStream();
 if(baseCalls!=1||rootCalls!=1||bad)return 1;
 const unsigned char* bytes=(const unsigned char*)raw;for(unsigned i=4;i<sizeof(raw);++i)if(bytes[i]!=0x5a)return 2;
 puts("MEMORY_STREAM_DTOR_PASS");return 0;
}
