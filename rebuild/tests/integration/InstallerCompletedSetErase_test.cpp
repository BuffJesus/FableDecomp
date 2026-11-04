#include <stdio.h>
#include <stdlib.h>
#include "fable_file_installer_containers.h"
typedef InstallerCompletedFileSet::Node Node;
static Node* expected[7];static unsigned length,position,mode;
static void require(bool b){if(!b)abort();}
static void predict(Node* n){if(n){predict(n->right);expected[length++]=n;predict(n->left);}}
CCharString::~CCharString()
{
    Node* n=(Node*)((unsigned char*)this-16);
    require(position<length&&expected[position++]==n);
    if(mode)n->left=0;
}
int main()
{
    unsigned cases=0;
    for(unsigned cycle=0;cycle!=8;++cycle)for(mode=0;mode!=2;++mode)for(unsigned shape=0;shape!=4;++shape)
    {
        Node* nodes[7];unsigned count=shape?7:0;
        for(unsigned i=0;i<count;++i){nodes[i]=(Node*)calloc(1,sizeof(Node));require(nodes[i]!=0);}
        if(shape==1){for(i=1;i<count;++i)nodes[i-1]->left=nodes[i];}
        if(shape==2){for(i=1;i<count;++i)nodes[i-1]->right=nodes[i];}
        if(shape==3){for(i=0;i!=3;++i){nodes[i]->left=nodes[2*i+1];nodes[i]->right=nodes[2*i+2];}}
        Node* root=count?nodes[0]:0;length=position=0;predict(root);InstallerCompletedFileSet set;set.Erase(root);
        require(position==count);++cases;
    }
    printf("INSTALLER_SET_ERASE_OK cases=%u\n",cases);return 0;
}
