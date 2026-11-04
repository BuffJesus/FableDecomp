#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_file_installer_active_files.h"
typedef InstallerActiveFileList::Node Node;
typedef InstallerTransferList::Link Link;
static Node* nodes[2];static CCPPointerInfo *oldInfo[2],*newInfo[2];
struct Event {unsigned kind;void* pointer;};
static Event expected[32];static unsigned length,position,mode,nodeCount,memberCount,refcase;
static void require(bool ok){if(!ok)abort();}
static void append(unsigned kind,void* pointer){expected[length].kind=kind;expected[length++].pointer=pointer;}
static void event(unsigned kind,void* pointer){require(position<length&&expected[position].kind==kind&&expected[position].pointer==pointer);++position;}
static void FABLE_FASTCALL Destroy(void* pointer){event(3,pointer);}
extern "C" void __cdecl InfoDelete(void* p){event(4,p);free(p);}
extern "C" void __cdecl Fnod(void* p)
{
    event(1,p);
    for(unsigned i=0;i<nodeCount;++i)if(p==nodes[i])require(nodes[i]->file.Data==0&&nodes[i]->file.Info==0);
    free(p);
}
extern "C" void __fastcall CDtor(void* p)
{
    event(2,p);
    Node* node=(Node*)((unsigned char*)p-0x40);
    if(mode==1)node->next=0;
    if(mode==2)for(unsigned i=0;i<nodeCount;++i)if(node==nodes[i])node->file.Info=newInfo[i];
}
static void list(InstallerTransferList& list)
{
    Link* head=(Link*)malloc(sizeof(Link));require(head!=0);list.head=head;
    Link* last=head;
    for(unsigned i=0;i<memberCount;++i){Link* link=(Link*)malloc(sizeof(Link));require(link!=0);last->next=link;link->previous=last;last=link;append(1,link);}
    last->next=head;head->previous=last;if(!memberCount)head->next=head;append(1,head);
}
int main()
{
    unsigned cases=0;
    static const unsigned counts[5]={0,0,1,2,0xffffffff};
    for(unsigned cycle=0;cycle!=8;++cycle)for(nodeCount=0;nodeCount!=3;++nodeCount)for(memberCount=0;memberCount!=3;++memberCount)for(refcase=0;refcase!=5;++refcase)for(mode=0;mode!=3;++mode)
    {
        unsigned long sentinelStorage[sizeof(Node)/sizeof(unsigned long)];
        Node& sentinel=*reinterpret_cast<Node*>(sentinelStorage);
        InstallerActiveFileList owner;owner.head=&sentinel;sentinel.next=sentinel.previous=&sentinel;length=position=0;
        for(unsigned i=0;i<nodeCount;++i){
            nodes[i]=(Node*)malloc(sizeof(Node));require(nodes[i]!=0);memset(nodes[i],0xa5,sizeof(Node));
            ::new(&nodes[i]->file)InstallerActiveFileList::Value();
            oldInfo[i]=refcase?new CCPPointerInfo(Destroy,(void*)(0x4000+i*16)):0;newInfo[i]=mode==2?new CCPPointerInfo(Destroy,(void*)(0x8000+i*16)):0;
            if(oldInfo[i])oldInfo[i]->RefCount=counts[refcase];
            nodes[i]->file.Data=(const CFileInstaller::CFileInfo*)0x5000;nodes[i]->file.Info=oldInfo[i];
            list(nodes[i]->output);list(nodes[i]->input);append(2,&nodes[i]->compressor);
            if(mode==2||refcase==2){append(3,(void*)((mode==2?0x8000:0x4000)+i*16));append(4,mode==2?(void*)newInfo[i]:(void*)oldInfo[i]);}append(1,nodes[i]);
            nodes[i]->previous=i?nodes[i-1]:&sentinel;if(i)nodes[i-1]->next=nodes[i];
        }
        if(nodeCount){sentinel.next=nodes[0];sentinel.previous=nodes[nodeCount-1];nodes[nodeCount-1]->next=&sentinel;}
        owner.Clear();require(position==length&&sentinel.next==&sentinel&&sentinel.previous==&sentinel);
        if(mode==2||refcase!=2)for(i=0;i<nodeCount;++i){
            if(oldInfo[i]&&mode!=2)require(oldInfo[i]->RefCount==(counts[refcase]-1));
            delete oldInfo[i];
        }++cases;
    }
    printf("INSTALLER_ACTIVE_LIST_OK cases=%u\n",cases);return 0;
}
