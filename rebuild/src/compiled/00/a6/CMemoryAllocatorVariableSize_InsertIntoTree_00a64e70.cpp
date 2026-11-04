#include "fable_variable_size_allocator.h"

void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root,CMemoryArea* area)
{
    CMemoryArea* parent=0;
    bool left=false;
    while(root) {
        parent=root;
        if(area->DataSize==root->DataSize) {
            if(area<root) {
                root=root->PLeftChild;
                left=true;
            } else {
                root=root->PRightChild;
                left=false;
            }
        } else if(area->DataSize<root->DataSize) {
            root=root->PLeftChild;
            left=true;
        } else {
            root=root->PRightChild;
            left=false;
        }
    }
    area->PParentArea=parent;
    if(!parent)PTreeRoot=area;
    else if(left)parent->PLeftChild=area;
    else parent->PRightChild=area;
    area->Colour=0;
    InsertFixup(area);
}
