#include "fable_ui_bank_stream.h"
unsigned __fastcall FableUiBankStreamPosition(CFileDataInputStream* stream,void*)
{ return FableUiStreamPrefix(stream)->StreamPos; }
