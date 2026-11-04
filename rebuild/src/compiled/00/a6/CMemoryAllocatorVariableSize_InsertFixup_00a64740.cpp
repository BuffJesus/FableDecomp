#include "fable_variable_size_allocator.h"

inline void CMemoryAllocatorVariableSize::RotateLeft(CMemoryArea* area)
{
    CMemoryArea* child=area->PRightChild;
    area->PRightChild=child->PLeftChild;
    if(child->PLeftChild)child->PLeftChild->PParentArea=area;
    child->PParentArea=area->PParentArea;
    if(!area->PParentArea)PTreeRoot=child;
    else if(area==area->PParentArea->PLeftChild)area->PParentArea->PLeftChild=child;
    else area->PParentArea->PRightChild=child;
    child->PLeftChild=area;
    area->PParentArea=child;
}

inline void CMemoryAllocatorVariableSize::RotateRight(CMemoryArea* area)
{
    CMemoryArea* child=area->PLeftChild;
    area->PLeftChild=child->PRightChild;
    if(child->PRightChild)child->PRightChild->PParentArea=area;
    child->PParentArea=area->PParentArea;
    if(!area->PParentArea)PTreeRoot=child;
    else if(area==area->PParentArea->PLeftChild)area->PParentArea->PLeftChild=child;
    else area->PParentArea->PRightChild=child;
    child->PRightChild=area;
    area->PParentArea=child;
}

void CMemoryAllocatorVariableSize::InsertFixup(CMemoryArea* area)
{
    while(area!=PTreeRoot && area->PParentArea->Colour==0 && area->PParentArea!=PTreeRoot) {
        if(area->PParentArea==area->PParentArea->PParentArea->PLeftChild) {
            CMemoryArea* uncle=area->PParentArea->PParentArea->PRightChild;
            if(uncle && uncle->Colour==0) {
                area->PParentArea->Colour=1;
                uncle->Colour=1;
                area=area->PParentArea->PParentArea;
                area->Colour=0;
            } else {
                if(area==area->PParentArea->PRightChild) {
                    area=area->PParentArea;
                    RotateLeft(area);
                }
                CMemoryArea* parent=area->PParentArea;
                parent->Colour=1;
                CMemoryArea* grandparent=parent->PParentArea;
                grandparent->Colour=0;
                RotateRight(grandparent);
            }
        } else {
            CMemoryArea* uncle=area->PParentArea->PParentArea->PLeftChild;
            if(uncle && uncle->Colour==0) {
                area->PParentArea->Colour=1;
                uncle->Colour=1;
                area=area->PParentArea->PParentArea;
                area->Colour=0;
            } else {
                if(area==area->PParentArea->PLeftChild) {
                    area=area->PParentArea;
                    RotateRight(area);
                }
                CMemoryArea* parent=area->PParentArea;
                parent->Colour=1;
                CMemoryArea* grandparent=parent->PParentArea;
                grandparent->Colour=0;
                RotateLeft(grandparent);
            }
        }
    }
    PTreeRoot->Colour=1;
}
