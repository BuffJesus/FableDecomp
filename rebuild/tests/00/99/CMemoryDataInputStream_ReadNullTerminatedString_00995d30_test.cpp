#include "test_support/string_vector_fixture.h"
#include "fable_data_input_stream.h"
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CDataInputStream::CDataInputStream(){StreamPos=0;StreamSize=0;CurrentSourcePtr=0;SourceChunkStreamPos=0;SourceChunkBytesRemaining=0;}
CMemoryDataInputStream::~CMemoryDataInputStream(){}
void CDataInputStream::SetPosition(unsigned long){abort();}
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"
class ProbeStream : public CMemoryDataInputStream {
public:
 mutable int queries;unsigned long secondPosition;bool overrideSecond;
 ProbeStream():queries(0),secondPosition(0),overrideSecond(false){}
 virtual unsigned long GetPosition()const{++queries;return overrideSecond && queries%2==0?secondPosition:StreamPos;}
};
int main(){
 unsigned char text[]="one\0\0third\0";ProbeStream s;s.StreamSource=text;s.CurrentSourcePtr=text;s.StreamSize=sizeof(text);s.SourceChunkBytesRemaining=sizeof(text);s.SourceChunkStreamPos=123;
 const char* expected[]={"one","","third"};unsigned long positions[]={4,5,11};
 for(int i=0;i<3;++i){CCharString value=s.ReadNullTerminatedString();if(strcmp(value.GetConstCharPtr(),expected[i])||s.StreamPos!=positions[i]||s.CurrentSourcePtr!=text+positions[i]||s.SourceChunkBytesRemaining!=(long)(sizeof(text)-positions[i])||s.SourceChunkStreamPos!=123)return 1;}
 if(s.queries!=6||live||dataLive)return 2;
 s.StreamPos=0;s.CurrentSourcePtr=text;s.SourceChunkBytesRemaining=sizeof(text);s.overrideSecond=true;s.secondPosition=0x7fffffff;
 {CCharString value=s.ReadNullTerminatedString();if(strcmp(value.GetConstCharPtr(),"one")||s.StreamPos||s.CurrentSourcePtr!=text||s.SourceChunkBytesRemaining!=sizeof(text))return 3;}
 // Unsigned position arithmetic wraps before the 64-bit range guard, as retail does.
 s.secondPosition=0xfffffffe;
 {CCharString value=s.ReadNullTerminatedString();if(s.StreamPos!=2||s.CurrentSourcePtr!=text+2||strcmp(value.GetConstCharPtr(),"one"))return 4;}
 if(live||dataLive||s.queries!=10)return 5;puts("MEMORY_READ_STRING_PASS");return 0;
}
