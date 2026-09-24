#include "fable_ui_bank_registry.h"
FableUiWideStringValue* __fastcall FableUiFindBankPath(void* receiver,void*,FableUiWideStringValue* result,const FableUiStringValue* name)
{
    FableUiBankRegistryView* registry=static_cast<FableUiBankRegistryView*>(receiver);
    FableUiStringMapNode* alias=FableUiFindBankAliasNode(&registry->Aliases,0,name);
    FableUiStringValue resolved;
    FableUiCopyString(&resolved,0,alias==reinterpret_cast<FableUiStringMapNode*>(registry->Aliases.Head) ? name : &reinterpret_cast<FableUiBankAliasNode*>(alias)->Value);
    FableUiStringMapNode* node=FableUiFindBankPathNode(&registry->Paths,0,&resolved);
    if(node!=reinterpret_cast<FableUiStringMapNode*>(registry->Paths.Head))
    {
        FableUiConcatWideStrings(result,&registry->BasePath,&reinterpret_cast<FableUiBankPathNode*>(node)->Value);
        FableUiDestroyBankName(&resolved,0);
        return result;
    }
    FableUiStringValue message;
    FableUiConcatStringLiteral(&message,&resolved," bank not found!");
    FableUiGetStringText(&message,0);
    // Retail throws the empty CGenericException (base CExceptionBase), size 1.
    // Its byte is padding, not the diagnostic pointer returned above. No local
    // string cleanup precedes the runtime throw in this recovered function.
    unsigned char exceptionPadding=0;
    FableUiThrowException(&exceptionPadding,FableUiGenericExceptionThrowInfo);
}
