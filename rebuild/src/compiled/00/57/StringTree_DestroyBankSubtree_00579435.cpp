#include "fable_ui_bank_storage.h"
void __fastcall FableUiDestroyBankStringTree(FableUiBankTree* tree,void*,FableUiBankTreeNode* node)
{
    while(node)
    {
        FableUiDestroyBankStringTree(tree,0,node->Right);
        FableUiBankTreeNode* left=node->Left;
        FableUiDestroyBankName(reinterpret_cast<FableUiStringValue*>(reinterpret_cast<unsigned char*>(node)+16),0);
        FableUiFreeArchiveArray(node);
        node=left;
    }
}
