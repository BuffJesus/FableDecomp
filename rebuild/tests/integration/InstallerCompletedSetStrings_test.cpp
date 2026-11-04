#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_file_installer_containers.h"
typedef InstallerCompletedFileSet::Node Node;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static void require(bool ok){if(!ok)abort();}
int main()
{
    unsigned cases=0;
    const long baseline=g_CCharStringInstanceCount_013BD800;
    for(unsigned cycle=0;cycle!=8;++cycle)for(unsigned mode=0;mode!=3;++mode)for(unsigned shape=0;shape!=4;++shape)
    {
        {
            CCharString shared("retained-owner",-1);
            CCharStringData* data;memcpy(&data,&shared,sizeof(data));
            Node* nodes[7];unsigned count=shape?7:0;
            for(unsigned i=0;i<count;++i) {
                nodes[i]=(Node*)calloc(1,sizeof(Node));require(nodes[i]!=0);
                if(mode==0)::new(&nodes[i]->value)CCharString("completed-file.big",-1);
                else if(mode==1)::new(&nodes[i]->value)CCharString(shared);
                else ::new(&nodes[i]->value)CCharString();
                require(!strcmp((const char*)nodes[i]->value,mode==0?"completed-file.big":mode==1?"retained-owner":""));
            }
            if(shape==1){for(i=1;i<count;++i)nodes[i-1]->left=nodes[i];}
            if(shape==2){for(i=1;i<count;++i)nodes[i-1]->right=nodes[i];}
            if(shape==3){for(i=0;i!=3;++i){nodes[i]->left=nodes[2*i+1];nodes[i]->right=nodes[2*i+2];}}
            require(g_CCharStringInstanceCount_013BD800==baseline+1+count);
            require(data->owners==(mode==1?count+1:1));
            InstallerCompletedFileSet set;set.Erase(count?nodes[0]:0);
            require(g_CCharStringInstanceCount_013BD800==baseline+1 && data->owners==1 && !strcmp((const char*)shared,"retained-owner"));
        }
        require(g_CCharStringInstanceCount_013BD800==baseline);++cases;
    }
    printf("INSTALLER_SET_STRINGS_OK cases=%u\n",cases);return 0;
}
