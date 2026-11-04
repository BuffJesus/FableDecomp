#include "fable_string.h"
#include "fable_data_input_stream.h"
CCharString CMemoryDataInputStream::ReadNullTerminatedString()
{
 unsigned long position = GetPosition();
 const char* text = reinterpret_cast<const char*>(StreamSource) + position;
 CCharString value(text, -1);
 long consumed = value.GetLength() + 1;
 if (consumed > 0) {
  __int64 next = GetPosition() + (unsigned long)consumed;
  if (next >= 0 && next <= 0x7fffffff) {
   long newPosition = (long)next;
   long oldPosition = StreamPos;
   CurrentSourcePtr += newPosition - oldPosition;
   SourceChunkBytesRemaining += oldPosition - newPosition;
   StreamPos = newPosition;
  }
 }
 return value;
}
