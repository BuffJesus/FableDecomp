#include "fable_ui_bank_decode.h"
#include <string.h>
FableUiStringValue* __fastcall FableUiReadArchiveString(CFileDataInputStream* stream,void*,FableUiStringValue* result)
{
    CDataInputStream* prefix=FableUiStreamPrefix(stream);
    // Valid archive callers supply a readable four-byte length within the
    // retail position bound. Beyond it retail leaves this local undefined.
    unsigned length;
    if(prefix->StreamPos+4u<=0x7fffffffu)
    {
        if(prefix->SourceChunkBytesRemaining>=4)
        {
            memcpy(&length,prefix->CurrentSourcePtr,4);
            prefix->CurrentSourcePtr=static_cast<unsigned char*>(prefix->CurrentSourcePtr)+4;
            prefix->SourceChunkBytesRemaining-=4; prefix->StreamPos+=4;
        }
        else FableUiReadBankStreamSlow(stream,0,&length,4);
    }
    if(!length) return FableUiConstructBankName(result,0,"",-1);
    FableUiGraphicArray bytes;
    FableUiConstructArchiveBytes(&bytes,0,length+1u);
    void* target=bytes.End!=bytes.Begin ? bytes.Begin : &bytes;
    if(static_cast<long>(length)>0 && prefix->StreamPos+length<=0x7fffffffu)
    {
        if(static_cast<long>(length)<=prefix->SourceChunkBytesRemaining)
        {
            memcpy(target,prefix->CurrentSourcePtr,length);
            prefix->CurrentSourcePtr=static_cast<unsigned char*>(prefix->CurrentSourcePtr)+length;
            prefix->SourceChunkBytesRemaining-=length; prefix->StreamPos+=length;
        }
        else FableUiReadBankStreamSlow(stream,0,target,length);
    }
    static_cast<char*>(bytes.Begin)[length]=0;
    const char* text=bytes.End!=bytes.Begin ? static_cast<const char*>(bytes.Begin) : reinterpret_cast<const char*>(&bytes);
    FableUiStringValue temporary;
    FableUiConstructBankName(&temporary,0,text,-1);
    FableUiCopyString(result,0,&temporary);
    FableUiDestroyBankName(&temporary,0);
    if(bytes.Begin) FableUiFreeArchiveArray(bytes.Begin);
    return result;
}
