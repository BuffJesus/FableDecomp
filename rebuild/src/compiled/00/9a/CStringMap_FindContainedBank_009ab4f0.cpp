#include "fable_ui_bank_registry.h"
FableUiStringMapNode* __fastcall FableUiFindContainedBankNode(FableUiBankTree* tree,void*,const FableUiStringValue* key)
{ return FableUiFindStringMapNode(tree,key); }
