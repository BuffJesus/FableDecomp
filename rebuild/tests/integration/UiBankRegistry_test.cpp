#include "fable_ui_bank_registry.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
FableUiBankRegistryView FableUiBankRegistryState;
static FableUiBankTreeNode heads[3];
static FableUiBankHeaderNode nodes[3][10];
static FableUiRegisteredBank banks[2];
static FableUiRegisteredBankNode list[3];
static FableReferenceCount refs[4];
static CCharStringData records[12];
static const char* texts[12]={0,"\x80","\xFF","","a","aa","ab","b","bank","z","a","ab\0tail"};
static unsigned seed;
static unsigned BankId(void* p) { return p==&banks[0] ? 1 : p==&banks[1] ? 2 : 9; }
static unsigned RefId(void* p) { return p ? static_cast<unsigned>(static_cast<FableReferenceCount*>(p)-refs)+1 : 0; }
static void __fastcall Destroy(void* p)
{
    unsigned id=BankId(p); printf(" D%u",id);
    if((seed&16) && id<=2) list[id].Bank.Info=&refs[3];
}
void __cdecl FableUiDeleteReference(FableReferenceCount* p) { printf(" F%u",RefId(p)); }
void* __cdecl FableUiAllocateStringRecord(unsigned) { abort(); return 0; }
void* __cdecl FableUiAllocateStringBuffer(unsigned) { abort(); return 0; }
void __cdecl FableUiFreeStringRecord(void*) { abort(); }
void __cdecl FableUiFreeStringBuffer(void*) { abort(); }
static void InitializeRecords()
{
    memset(records,0xA5,sizeof(records));
    for(unsigned i=1;i<12;++i) { records[i].text=const_cast<char*>(texts[i]); records[i].unknown04=i==11 ? 7 : static_cast<unsigned>(strlen(texts[i])); records[i].owners=10; }
    g_CCharStringInstanceCount_013BD800=200;
}
static void BuildTree(unsigned which,unsigned mask)
{
    memset(&heads[which],0xA5,sizeof(heads[which])); heads[which].Parent=0;
    memset(nodes[which],0xA5,sizeof(nodes[which]));
    // Rotated insertion order gives different tree shapes, including chains.
    for(unsigned j=0;j<10;++j)
    {
        unsigned key=(j+seed)%10; if(!(mask&(1u<<key))) continue;
        FableUiBankHeaderNode* node=&nodes[which][key]; node->Node.Key.Storage=key ? &records[key] : 0;
        node->Node.Links.Left=node->Node.Links.Right=0;
        FableUiBankTreeNode** link=&heads[which].Parent;
        while(*link) { unsigned index=static_cast<unsigned>(reinterpret_cast<FableUiBankHeaderNode*>(*link)-nodes[which]); link=key<index ? &(*link)->Left : &(*link)->Right; }
        *link=&node->Node.Links;
        node->Value.Type=which*100+key; node->Value.EntryCount=0xFFFFFF00+key; node->Value.Offset=which*4096+key*16; node->Value.Unrecovered0C=0x12345678; node->Value.Alignment=1u<<(key%8);
    }
}
static unsigned NodeId(FableUiStringMapNode* p,unsigned which)
{ return p==reinterpret_cast<FableUiStringMapNode*>(&heads[which]) ? 99 : static_cast<unsigned>(reinterpret_cast<FableUiBankHeaderNode*>(p)-nodes[which]); }
int main()
{
    InitializeRecords();
    for(unsigned a=1;a<12;++a) for(unsigned b=1;b<12;++b) printf("CMP%u:%u:%u\n",a,b,FableUiStringDataLess(&records[a],0,&records[b]));
    for(seed=0;seed<64;++seed) for(unsigned q=0;q<12;++q)
    {
        InitializeRecords(); BuildTree(0,((seed*73)^0x2AD)&1023);
        FableUiBankTree tree={&heads[0],0,0}; FableUiStringValue query={q ? &records[q] : 0};
        printf("FIND%u:%u:%u\n",NodeId(FableUiFindContainedBankNode(&tree,0,&query),0),NodeId(FableUiFindBankPathNode(&tree,0,&query),0),NodeId(FableUiFindBankAliasNode(&tree,0,&query),0));
    }
    for(seed=0;seed<64;++seed) for(unsigned q=0;q<12;++q)
    {
        InitializeRecords(); memset(banks,0xA5,sizeof(banks)); memset(list,0,sizeof(list));
        for(unsigned i=0;i<4;++i) { refs[i].owners=(i<2 && (seed&16)) ? 0 : 1; refs[i].destroy=Destroy; refs[i].object=i<2 ? &banks[i] : reinterpret_cast<void*>(0x12345678); }
        unsigned aliasMask=(seed&1 ? 1u<<4 : 0)|(seed&2 ? 1 : 0)|(seed&4 ? 1u<<8 : 0);
        BuildTree(0,aliasMask); unsigned target0=3,target4=7,target8=9;
        reinterpret_cast<FableUiBankAliasNode*>(&nodes[0][0])->Value.Storage=&records[target0];
        reinterpret_cast<FableUiBankAliasNode*>(&nodes[0][4])->Value.Storage=&records[target4];
        reinterpret_cast<FableUiBankAliasNode*>(&nodes[0][8])->Value.Storage=&records[target8];
        FableUiBankRegistryState.Aliases.Head=&heads[0];
        unsigned count=seed%3; list[0].Next=count ? &list[1] : &list[0]; list[0].Previous=count ? &list[count] : &list[0];
        FableUiBankRegistryState.Files=&list[0];
        for(i=0;i<2;++i)
        {
            BuildTree(i+1,(((seed+17*i)*73)^0x2AD)&1023); banks[i].Banks.Head=&heads[i+1];
            list[i+1].Next=i+1<count ? &list[i+2] : &list[0]; list[i+1].Previous=&list[i];
            list[i+1].Bank.Data=&banks[i]; list[i+1].Bank.Info=seed&8 ? 0 : &refs[i];
        }
        FableUiBankReference out={reinterpret_cast<void*>(0x12345678),seed&32 ? &refs[0] : &refs[2]};
        FableUiRegisteredBankHeader header; memset(&header,0xA5,sizeof(header)); FableUiStringValue query={q ? &records[q] : 0};
        printf("TRACE"); bool found=FableUiFindRegisteredBank(&FableUiBankRegistryState,0,&query,&header,&out);
        printf(" RESULT%u:%u:%u H",found,BankId(out.Data),RefId(out.Info));
        for(i=0;i<5;++i) printf(":%08x",reinterpret_cast<unsigned*>(&header)[i]);
        printf(" REFS"); for(i=0;i<4;++i) printf(":%d",refs[i].owners);
        printf(" LIST:%u:%u STR:%d",RefId(list[1].Bank.Info),RefId(list[2].Bank.Info),g_CCharStringInstanceCount_013BD800);
        for(i=1;i<12;++i) printf(":%d",records[i].owners);
        printf(" END\n");
    }
    return 0;
}
