#include "fable_ui_bank_registry.h"
bool __fastcall FableUiFindRegisteredBank(void* receiver,void*,const FableUiStringValue* name,FableUiRegisteredBankHeader* header,FableUiBankReference* out)
{
    FableUiBankRegistryView* registry=static_cast<FableUiBankRegistryView*>(receiver);
    FableUiStringMapNode* alias=FableUiFindBankAliasNode(&registry->Aliases,0,name);
    const FableUiStringValue* target=alias==reinterpret_cast<FableUiStringMapNode*>(registry->Aliases.Head) ? name : &reinterpret_cast<FableUiBankAliasNode*>(alias)->Value;
    FableUiStringValue resolved;
    FableUiCopyString(&resolved,0,target);
    for(FableUiRegisteredBankNode* node=registry->Files->Next;node!=registry->Files;node=node->Next)
    {
        FableUiRegisteredBank* bank=static_cast<FableUiRegisteredBank*>(node->Bank.Data);
        FableReferenceCount* info=static_cast<FableReferenceCount*>(node->Bank.Info);
        if(info) ++info->owners;
        FableUiStringMapNode* found=FableUiFindContainedBankNode(&bank->Banks,0,&resolved);
        if(found!=reinterpret_cast<FableUiStringMapNode*>(bank->Banks.Head))
        {
            *header=reinterpret_cast<FableUiBankHeaderNode*>(found)->Value;
            FableUiShareBankReference(out,0,bank,info);
            if(info && --info->owners==0) { info->destroy(info->object); FableUiDeleteReference(info); }
            FableUiDestroyBankName(&resolved,0);
            return true;
        }
        if(info && --info->owners==0) { info->destroy(info->object); FableUiDeleteReference(info); }
    }
    FableUiDestroyBankName(&resolved,0);
    return false;
}
