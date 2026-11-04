#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "fable_file_installer_containers.h"
typedef InstallerCountedFileList::Node Node;
static Node* nodes[3];static CCPPointerInfo *original[3],*replacement[3];
static unsigned nodeCount,position,callbacks,deletes,mode,refCase;
static InstallerCountedFileList* activeList;
static Node* alternateHead;
static void require(bool ok){if(!ok)abort();}
static void FABLE_FASTCALL Destroy(void* object)
{
    require(position<nodeCount && object==(void*)(0x4000+position*16));++callbacks;
    require(original[position]->RefCount==0);
    if(mode==1)nodes[position]->value.Info=replacement[position];
    if(mode==2)nodes[position]->next=0;
}
extern "C" void __cdecl InfoDelete(void* p)
{
    require(position<nodeCount && p==(mode==1?(void*)replacement[position]:(void*)original[position]));++deletes;
    if(mode==3){nodes[position]->value.Data=(const CFileInstaller::CFileInfo*)0x9999;nodes[position]->value.Info=0;}
    free(p);
}
extern "C" void __cdecl Fnod(void* p)
{
    require(position<nodeCount && p==nodes[position]);
    require(nodes[position]->value.Data==0 && nodes[position]->value.Info==0);
    if(refCase && refCase!=2)require(original[position]->RefCount==(refCase==1?0xffffffff:refCase==3?1:0xfffffffe));
    if(mode==4 && position==0 && nodeCount>1){activeList->head=alternateHead;nodes[nodeCount-1]->next=alternateHead;}
    ++position;free(p);
}
int main()
{
    unsigned cases=0;const unsigned counts[]={0,0,1,2,0xffffffff};
    for(unsigned cycle=0;cycle!=8;++cycle)for(nodeCount=0;nodeCount!=4;++nodeCount)for(refCase=0;refCase!=5;++refCase)for(mode=0;mode!=5;++mode)
    {
        Node sentinel;InstallerCountedFileList list;list.head=&sentinel;sentinel.next=sentinel.previous=&sentinel;
        Node alternate;alternateHead=&alternate;alternate.next=alternate.previous=0;activeList=&list;
        for(unsigned i=0;i<nodeCount;++i){
            nodes[i]=(Node*)malloc(sizeof(Node));require(nodes[i]!=0);::new(&nodes[i]->value)InstallerCountedFileList::Value();
            original[i]=refCase?(CCPPointerInfo*)malloc(sizeof(CCPPointerInfo)):0;
            replacement[i]=(refCase==2&&mode==1)?(CCPPointerInfo*)malloc(sizeof(CCPPointerInfo)):0;
            if(original[i]){original[i]->RefCount=counts[refCase];original[i]->DeleteFunc=Destroy;original[i]->Data=(void*)(0x4000+i*16);}
            if(replacement[i]){replacement[i]->RefCount=77;replacement[i]->DeleteFunc=Destroy;replacement[i]->Data=(void*)0x8888;}
            nodes[i]->value.Data=(const CFileInstaller::CFileInfo*)(0x5000+i*16);nodes[i]->value.Info=original[i];
            nodes[i]->previous=i?nodes[i-1]:&sentinel;if(i)nodes[i-1]->next=nodes[i];
        }
        if(nodeCount){sentinel.next=nodes[0];sentinel.previous=nodes[nodeCount-1];nodes[nodeCount-1]->next=&sentinel;}
        position=callbacks=deletes=0;list.Clear();
        Node* finalHead=(mode==4&&nodeCount>1)?&alternate:&sentinel;
        require(position==nodeCount && callbacks==(refCase==2?nodeCount:0) && deletes==callbacks && list.head==finalHead && finalHead->next==finalHead && finalHead->previous==finalHead);
        for(i=0;i<nodeCount;++i)if(original[i]&&(refCase!=2||mode==1))free(original[i]);
        ++cases;
    }
    printf("INSTALLER_COUNTED_LIST_OK cases=%u\n",cases);return 0;
}
