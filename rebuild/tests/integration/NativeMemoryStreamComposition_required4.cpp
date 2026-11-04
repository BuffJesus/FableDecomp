#include <stdlib.h>
#include "fable_data_input_stream.h"
extern "C" void UnusedMemoryBoundary(){abort();}
class CDataInputStreamTestAccess {public: static void Next(CDataInputStream& s){const fable_u8* p=0;long n=0;s.CDataInputStream::InternalReadNextSourceChunk(p,n);} static void Needs(CDataInputStream&s){s.CDataInputStream::NeedsBufferedRead(4);} static void Unbuffered(CDataInputStream&s,void*p){s.CDataInputStream::InternalReadUnbuffered((fable_u8*)p,4);}};
int main(int,char**argv){CDataInputStreamTestAccess::Next(*(CDataInputStream*)argv);return 0;}
#pragma comment(linker,"/include:?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z")
