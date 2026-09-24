#include "fable_ui_bank_stream.h"
bool __fastcall FableUiBankStreamUseBuffer(CFileDataInputStream* stream,void*,long length)
{ return length<stream->BufferSize; }
