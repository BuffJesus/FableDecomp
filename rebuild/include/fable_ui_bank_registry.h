#pragma once
#include "fable_ui_bank_open.h"
struct FableUiStringMapNode { FableUiBankTreeNode Links; FableUiStringValue Key; };
struct FableUiBankAliasNode { FableUiStringMapNode Node; FableUiStringValue Value; };
struct FableUiBankPathNode { FableUiStringMapNode Node; FableUiWideStringValue Value; };
struct FableUiBankHeaderNode { FableUiStringMapNode Node; FableUiRegisteredBankHeader Value; };
struct FableUiRegisteredBankNode
{
    FableUiRegisteredBankNode* Next;
    FableUiRegisteredBankNode* Previous;
    FableUiBankReference Bank;
};
struct FableUiBankRegistryView
{
    void* Vtable;
    FableUiBankTree Paths;
    FableUiRegisteredBankNode* Files;
    bool RetailMode;
    unsigned char Unrecovered15[3];
    FableUiBankTree Aliases;
    FableUiWideStringValue BasePath;
};
FABLE_STATIC_ASSERT(offsetof(FableUiBankRegistryView,Files)==0x10);
FABLE_STATIC_ASSERT(offsetof(FableUiBankRegistryView,Aliases)==0x18);
FABLE_STATIC_ASSERT(offsetof(FableUiBankRegistryView,BasePath)==0x24);
FABLE_STATIC_ASSERT(offsetof(FableUiRegisteredBank,Banks)==0x18);
FABLE_STATIC_ASSERT(offsetof(FableUiBankHeaderNode,Value)==0x14);
// Recovered character access and the remaining exception-runtime boundary.
const char* __fastcall FableUiGetStringText(const FableUiStringValue*,void*); // 0099E4C0
extern const char FableUiEmptyString[]; // 0129AAF4
extern const unsigned char FableUiGenericExceptionThrowInfo[]; // 013692F8
__declspec(noreturn) void __stdcall FableUiThrowException(void*,const void*); // 00BFEB84
bool __fastcall FableUiStringDataLess(const CCharStringData*,void*,const CCharStringData*); // 00429950
FableUiStringMapNode* __fastcall FableUiFindContainedBankNode(FableUiBankTree*,void*,const FableUiStringValue*); // 009AB4F0
FableUiStringMapNode* __fastcall FableUiFindBankPathNode(FableUiBankTree*,void*,const FableUiStringValue*); // 009AB560
FableUiStringMapNode* __fastcall FableUiFindBankAliasNode(FableUiBankTree*,void*,const FableUiStringValue*); // 009AB5D0
inline bool FableUiNullableStringLess(CCharStringData* left,CCharStringData* right)
{
    if(left==right) return false;
    if(!left) return true;
    if(!right) return false;
    return FableUiStringDataLess(left,0,right);
}
inline FableUiStringMapNode* FableUiFindStringMapNode(FableUiBankTree* tree,const FableUiStringValue* key)
{
    FableUiBankTreeNode* candidate=tree->Head;
    FableUiBankTreeNode* node=candidate->Parent;
    CCharStringData* query=key->Storage;
    while(node)
    {
        if(!FableUiNullableStringLess(reinterpret_cast<FableUiStringMapNode*>(node)->Key.Storage,query))
        { candidate=node; node=node->Left; }
        else node=node->Right;
    }
    if(candidate!=tree->Head && FableUiNullableStringLess(key->Storage,reinterpret_cast<FableUiStringMapNode*>(candidate)->Key.Storage)) candidate=tree->Head;
    return reinterpret_cast<FableUiStringMapNode*>(candidate);
}
