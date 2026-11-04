#include "fable_def_string.h"
#include "fable_data_input_stream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Raw receiver storage isolates the read; no stream construction/vtable dispatch occurs.
static int overflows; static long overflowValue;
void CDataInputStream::ReadWithSrcChunkOverflow(void* out,long size) {
 if(size!=4) abort(); ++overflows; memcpy(out,&overflowValue,4); StreamPos+=4;
}
int main() {
 double raw[(sizeof(CDataInputStream)+7)/8];
 CDataInputStream& s=*reinterpret_cast<CDataInputStream*>(raw);
 struct Guarded { unsigned long before; CDefString value; unsigned long after; } g;
 const long values[]={0,-1,0x12345678,(-2147483647-1)};
 for(int i=0;i<4;++i) {
  memset(raw,0,sizeof(raw)); unsigned char bytes[12]; memset(bytes,0xa5,sizeof(bytes)); memcpy(bytes+1,&values[i],4);
  s.StreamPos=100; s.StreamSize=200; s.CurrentSourcePtr=bytes+1; s.SourceChunkBytesRemaining=7; s.SourceChunkStreamPos=99;
  g.before=0x13579;g.after=0x24680; g.value.TablePos=77;overflows=0;
  g.value.TransferBinaryIn(s);
  if(g.value.TablePos!=values[i] || s.StreamPos!=104 || s.CurrentSourcePtr!=bytes+5 || s.SourceChunkBytesRemaining!=3 || overflows || s.SourceChunkStreamPos!=99 || s.StreamSize!=200 || g.before!=0x13579 || g.after!=0x24680)return 1;
 }
 for(int avail=0;avail<4;++avail) {
  memset(raw,0,sizeof(raw));s.StreamPos=21;s.StreamSize=100;s.SourceChunkBytesRemaining=avail;overflowValue=0x654321;overflows=0;
  g.value.TransferBinaryIn(s);
  if(g.value.TablePos!=overflowValue || s.StreamPos!=25 || overflows!=1)return 2;
 }
 // Retail suppresses the read beyond signed position range. The returned local
 // is unspecified there; verify only the observable stream/call side effects.
 memset(raw,0,sizeof(raw));s.StreamPos=0x7ffffffe;s.StreamSize=-1;s.SourceChunkBytesRemaining=0;overflows=0;
 g.value.TransferBinaryIn(s);if(s.StreamPos!=0x7ffffffe || overflows)return 3;
 puts("DEF_STRING_TRANSFER_PASS");return 0;
}