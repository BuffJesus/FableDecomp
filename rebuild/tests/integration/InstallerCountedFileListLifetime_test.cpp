#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "fable_file_installer_containers.h"
typedef InstallerCountedFileList::Node Node;
struct Object { unsigned id; };
static unsigned destroyed[3];
static void require(bool ok){if(!ok)abort();}
static void FABLE_FASTCALL Destroy(void* value)
{
    Object* object=(Object*)value;require(object && object->id<3 && destroyed[object->id]==0);
    ++destroyed[object->id];delete object;
}
int main()
{
    unsigned cases=0,objects=0;
    for(unsigned cycle=0;cycle!=32;++cycle)for(unsigned owners=1;owners!=4;++owners)
    {
        InstallerCountedFileList lists[3];Node sentinel[3];
        Object* object=new Object;object->id=0;
        CCPPointerInfo* info=new CCPPointerInfo(Destroy,object);info->RefCount=owners;
        for(unsigned i=0;i!=3;++i)destroyed[i]=0;
        for(i=0;i!=owners;++i){
            lists[i].head=&sentinel[i];Node* node=(Node*)malloc(sizeof(Node));require(node!=0);
            ::new(&node->value)InstallerCountedFileList::Value();
            node->value.Data=(const CFileInstaller::CFileInfo*)object;node->value.Info=info;
            node->next=node->previous=&sentinel[i];sentinel[i].next=sentinel[i].previous=node;
        }
        for(i=0;i!=owners;++i){
            lists[i].Clear();require(sentinel[i].next==&sentinel[i]&&sentinel[i].previous==&sentinel[i]);
            if(i+1<owners)require(info->RefCount==owners-i-1&&destroyed[0]==0&&object->id==0);
            else require(destroyed[0]==1);
            lists[i].Clear();require(destroyed[0]==(i+1==owners?1U:0U));
        }
        ++objects;++cases;
    }
    printf("INSTALLER_COUNTED_LIST_LIFETIME_OK cases=%u objects=%u\n",cases,objects);return 0;
}
