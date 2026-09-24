#include "fable_ui_bank_entries.h"
#include <string.h>
namespace
{
void Read(CFileDataInputStream* stream,void* target,unsigned count)
{
    CDataInputStream* prefix=FableUiStreamPrefix(stream);
    if(static_cast<long>(count)<=0 || prefix->StreamPos+count>0x7fffffffu) return;
    if(static_cast<long>(count)<=prefix->SourceChunkBytesRemaining)
    {
        memcpy(target,prefix->CurrentSourcePtr,count);
        prefix->CurrentSourcePtr=static_cast<unsigned char*>(prefix->CurrentSourcePtr)+count;
        prefix->SourceChunkBytesRemaining-=count; prefix->StreamPos+=count;
    }
    else FableUiReadBankStreamSlow(stream,0,target,count);
}
unsigned Word(CFileDataInputStream* stream) { unsigned value; Read(stream,&value,4); return value; }
}
bool __fastcall FableUiReadBankEntries(FableUiBankFileView* self,void*,CFileDataInputStream* stream,unsigned size,unsigned count)
{
    FableUiPrepareBankStorage(self,0,size+1);
    unsigned pairCount=Word(stream);
    FableUiGraphicArray pairs;
    FableUiConstructArchivePairs(&pairs,0,pairCount);
    for(unsigned p=0;p<pairCount;++p)
    {
        static_cast<unsigned*>(pairs.Begin)[p*2]=Word(stream);
        static_cast<unsigned*>(pairs.Begin)[p*2+1]=Word(stream);
    }
    typedef void (__fastcall *TableCallback)(FableUiBankFileView*,void*,unsigned,FableUiGraphicArray*);
    reinterpret_cast<TableCallback*>(self->Vtable)[10](self,0,self->Size,&pairs);
    for(unsigned i=0;i<count;++i)
    {
        Word(stream); // Serialized field not used by retail's runtime representation.
        unsigned index=Word(stream),type=Word(stream),length=Word(stream),offset=Word(stream),metadata=Word(stream);
        FableUiStringValue symbol;
        FableUiReadArchiveString(stream,0,&symbol);
        unsigned checksum=Word(stream),aliasCount=Word(stream);
        FableUiBankAliases aliases; aliases.Data=0; aliases.Count=0;
        FableUiResizeBankAliases(&aliases,0,aliasCount);
        for(unsigned a=0;a<aliasCount;++a)
        {
            FableUiStringValue name;
            FableUiReadArchiveString(stream,0,&name);
            FableUiAssignString(aliases.Data+a,0,&name);
            FableUiDestroyBankName(&name,0);
        }
        unsigned blobLength=Word(stream);
        FableUiGraphicArray blob;
        FableUiConstructArchiveBytes(&blob,0,blobLength);
        Read(stream,blob.Begin!=blob.End ? blob.Begin : &blob,blobLength);
        FableUiBankRuntimeEntry* entry=static_cast<FableUiBankRuntimeEntry*>(self->RuntimeData.Begin)+index;
        entry->Type=static_cast<unsigned char>(type); entry->Size=length; entry->Offset=offset; entry->Valid=1;
        if(self->OpenFlags&8) FableUiAssignString(static_cast<FableUiStringValue*>(self->Symbols.Begin)+index,0,&symbol);
        if(self->OpenFlags&0x10) static_cast<unsigned*>(self->Checksums.Begin)[index]=checksum;
        if(self->OpenFlags&0x20)
        {
            FableUiBankUpdateEntry* update=static_cast<FableUiBankUpdateEntry*>(FableUiAllocateStringRecord(24));
            update->Metadata=0; update->Length=0; update->Bytes=0; update->Aliases.Data=0; update->Aliases.Count=0;
            update->Unrecovered14=false; update->Unrecovered15=true;
            update->Metadata=metadata;
            update->Length=static_cast<unsigned char*>(blob.End)-static_cast<unsigned char*>(blob.Begin);
            if(update->Length)
            {
                update->Bytes=FableUiAllocateStringBuffer(update->Length);
                memcpy(update->Bytes,blob.Begin!=blob.End ? blob.Begin : &blob,update->Length);
            }
            FableUiCopyBankAliases(&update->Aliases,0,&aliases);
            static_cast<FableUiBankUpdateEntry**>(self->UpdateData.Begin)[index]=update;
        }
        FableUiStringValue first;
        if(aliases.Count) FableUiCopyString(&first,0,aliases.Data);
        else
        {
            FableUiStringValue empty;
            FableUiConstructBankName(&empty,0,"",-1);
            FableUiCopyString(&first,0,&empty);
            FableUiDestroyBankName(&empty,0);
        }
        FableUiRegisterBankEntry(self,0,index,&symbol,&first);
        FableUiBankStreamVtable* methods=static_cast<FableUiBankStreamVtable*>(FableUiStreamPrefix(stream)->__vftable);
        unsigned position=methods->GetPosition(stream,0);
        typedef void (__fastcall *EntryCallback)(FableUiBankFileView*,void*,unsigned,unsigned,FableUiGraphicArray*);
        reinterpret_cast<EntryCallback*>(self->Vtable)[12](self,0,type,index,&blob);
        typedef void (__fastcall *Seek)(CFileDataInputStream*,void*,unsigned);
        reinterpret_cast<Seek*>(FableUiStreamPrefix(stream)->__vftable)[1](stream,0,position);
        FableUiDestroyBankName(&first,0);
        if(blob.Begin) FableUiFreeArchiveArray(blob.Begin);
        FableUiClearBankAliases(&aliases,0);
        FableUiDestroyBankName(&symbol,0);
    }
    if(self->SymbolCRCFlag)
    {
        FableUiSortBankChecksums(self->SymbolCRCIndices.Begin,self->SymbolCRCIndices.End,self->UnrecoveredDC);
        self->SymbolCRCFlag=false;
    }
    FableUiCompactBankChecksums(&self->SymbolCRCIndices,0);
    if(self->ReadOnly) FableUiPackBankRuntime(self,0);
    typedef void (__fastcall *Finish)(FableUiBankFileView*,void*);
    reinterpret_cast<Finish*>(self->Vtable)[15](self,0);
    self->FileValid=true;
    if(pairs.Begin) FableUiFreeArchiveArray(pairs.Begin);
    return true;
}
