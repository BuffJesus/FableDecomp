#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorVariableSize::SetTreeRootNode(CMemoryArea* area){PTreeRoot=area;}
inline CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::GetTreeRoot() const{return PTreeRoot;}

inline void CMemoryAllocatorVariableSize::RotateLeft(CMemoryArea* area)
{
    CMemoryArea* child=area->PRightChild;
    area->PRightChild=child->PLeftChild;
    if(child->PLeftChild)child->PLeftChild->PParentArea=area;
    child->PParentArea=area->PParentArea;
    if(!area->PParentArea)SetTreeRootNode(child);
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
    if(!area->PParentArea)SetTreeRootNode(child);
    else if(area==area->PParentArea->PLeftChild)area->PParentArea->PLeftChild=child;
    else area->PParentArea->PRightChild=child;
    child->PRightChild=area;
    area->PParentArea=child;
}

bool CMemoryAllocatorVariableSize::IsNodeBlack(const CMemoryArea* area) const
{
    return (area && area->Colour!=1)?0:1;
}
void CMemoryAllocatorVariableSize::RemoveFromTreeFixup(CMemoryArea* area,const CDeletedNodeChildInfo* info)
{
    while((area && area!=GetTreeRoot() && area->Colour==1) ||
          (info && info->Valid && info->PParent && info->Colour==1)) {
        CMemoryArea* parent;
        if(area)parent=area->PParentArea;
        else parent=info->PParent;
        if((area && area==parent->PLeftChild) || (info && info->LeftChild)) {
            CMemoryArea* sibling=parent->PRightChild;
            if(sibling->Colour==0) {
                sibling->Colour=1;
                parent->Colour=0;
                RotateLeft(parent);
                sibling=parent->PRightChild;
                if(!sibling) {
                    area=GetTreeRoot();
                    info=0;
                    continue;
                }
            }
            CMemoryArea* sib_left_child=sibling->PLeftChild;
            if(IsNodeBlack(sib_left_child) && IsNodeBlack(sibling->PRightChild)) {
                sibling->Colour=0;
                area=parent;
                info=0;
            } else {
                if(IsNodeBlack(sibling->PRightChild)) {
                    if(sib_left_child)sib_left_child->Colour=1;
                    sibling->Colour=0;
                    RotateRight(sibling);
                    sibling=parent->PRightChild;
                }
                sibling->Colour=parent->Colour;
                parent->Colour=1;
                CMemoryArea* sib_right_child=sibling->PRightChild;
                if(sib_right_child)sib_right_child->Colour=1;
                RotateLeft(parent);
                area=GetTreeRoot();
                info=0;
            }
        } else {
            CMemoryArea* sibling=parent->PLeftChild;
            if(sibling->Colour==0) {
                sibling->Colour=1;
                parent->Colour=0;
                RotateRight(parent);
                sibling=parent->PLeftChild;
                if(!sibling) {
                    area=GetTreeRoot();
                    info=0;
                    continue;
                }
            }
            CMemoryArea* sib_right_child=sibling->PRightChild;
            if(IsNodeBlack(sib_right_child) && IsNodeBlack(sibling->PLeftChild)) {
                sibling->Colour=0;
                area=parent;
                info=0;
            } else {
                if(IsNodeBlack(sibling->PLeftChild)) {
                    if(sib_right_child)sib_right_child->Colour=1;
                    sibling->Colour=0;
                    RotateLeft(sibling);
                    sibling=parent->PLeftChild;
                }
                sibling->Colour=parent->Colour;
                parent->Colour=1;
                CMemoryArea* sib_left_child=sibling->PLeftChild;
                if(sib_left_child)sib_left_child->Colour=1;
                RotateRight(parent);
                area=GetTreeRoot();
                info=0;
            }
        }
    }
    if(area)area->Colour=1;
}
