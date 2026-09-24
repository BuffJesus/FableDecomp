#include "fable_ui_bank_factory.h"
#include <string.h>

FableUiBankReference* __fastcall FableUiCreateGraphicsBank(void* receiver,void*,FableUiBankReference* output,const FableUiStringValue* name,const FableUiGraphicBankInit* init,bool,bool)
{
    FableUiGraphicsBankManagerView* manager=static_cast<FableUiGraphicsBankManagerView*>(receiver);
    FableUiGraphicsBankNode* head=manager->Banks;
    for(FableUiGraphicsBankNode* node=head->Next;node!=head;node=node->Next)
    {
        CCharStringData* a=node->Name.Storage;
        CCharStringData* b=name->Storage;
        if(a==b || (a && b && a->unknown04==b->unknown04 && FableUiCompareStringBytes(a->text,b->text)==0))
        {
            *output=node->Bank;
            if(output->Info) ++static_cast<FableReferenceCount*>(output->Info)->owners;
            return output;
        }
    }
    FableUiStringValue message;
    FableUiConstructBankName(&message,0,"Init Graphic Data Bank",-1);
    FableUiStartBankProgress(&message,false,-1.0f,false);
    FableUiDestroyBankName(&message,0);

    void* storage=FableUiAllocateGraphicsBank(0x30C);
    FableUiGraphicsBank* bank=storage ? FableUiConstructGraphicsBank(storage,0) : 0;
    FableReferenceCount* info=0;
    if(bank)
    {
        info=static_cast<FableReferenceCount*>(FableUiAllocateGraphicsBank(12));
        if(info) { info->owners=1; info->destroy=FableUiDestroyGraphicsBank; info->object=bank; }
    }
    FableUiInitialiseGraphicsBank(bank,0,init);
    unsigned flags=FableUiGraphicsBankOpenMode ? 0 : 0xD8;
    FableUiConstructBankName(&message,0,"Opening Graphic Data Bank",-1);
    FableUiStartBankProgress(&message,false,-1.0f,false);
    FableUiDestroyBankName(&message,0);
    bank->Vtable->Open(bank,0,name,flags);

    FableUiStringValue entryName;
    FableUiConstructEmptyString(&entryName,0);
    FableUiBankReference entryBank={0,0};
    FableUiAssignString(&entryName,0,name);
    FableUiGraphicBankInit configuration;
    memcpy(&configuration,init,sizeof(configuration));
    FableUiShareBankReference(&entryBank,0,bank,info);

    head=manager->Banks;
    FableUiGraphicsBankNode* inserted=static_cast<FableUiGraphicsBankNode*>(FableUiAllocateGraphicsBankNode(64));
    memcpy(&inserted->Init,&configuration,sizeof(configuration));
    FableUiCopyString(&inserted->Name,0,&entryName);
    inserted->Bank=entryBank;
    if(inserted->Bank.Info) ++static_cast<FableReferenceCount*>(inserted->Bank.Info)->owners;
    inserted->Next=head; inserted->Previous=head->Previous;
    head->Previous->Next=inserted; head->Previous=inserted;

    output->Data=bank; output->Info=info;
    if(info) ++info->owners;
    FableUiReleaseBankReference(&entryBank,0);
    FableUiDestroyBankName(&entryName,0);
    if(info && --info->owners==0) { info->destroy(info->object); FableUiDeleteReference(info); }
    return output;
}
