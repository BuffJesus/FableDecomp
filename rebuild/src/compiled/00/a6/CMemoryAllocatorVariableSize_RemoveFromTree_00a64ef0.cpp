#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorVariableSize::SetTreeRootNode(CMemoryArea* area){PTreeRoot=area;}
inline CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::GetTreeRoot() const{return PTreeRoot;}
void CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea* area)
{
    CMemoryArea* removed=0;
    unsigned char colour;
    if(!area->PLeftChild || !area->PRightChild)removed=area;
    else {
        removed=area->PRightChild;
        while(removed->PLeftChild)removed=removed->PLeftChild;
    }
    CDeletedNodeChildInfo info;
    info.Valid=true;
    info.PParent=removed->PParentArea;
    if(info.PParent==area)info.PParent=removed;
    if(info.PParent) {
        if(info.PParent->PLeftChild==removed)info.LeftChild=true;
        else info.LeftChild=false;
    }
    CMemoryArea* child=0;
    if(removed->PLeftChild) {
        child=removed->PLeftChild;
        info.LeftChild=true;
        info.Colour=child->Colour;
    } else {
        child=removed->PRightChild;
        if(!child)info.Colour=removed->Colour;
        else {
            info.LeftChild=false;
            info.Colour=child->Colour;
        }
    }
    if(child) {
        info.Valid=false;
        child->PParentArea=removed->PParentArea;
    }
    if(removed->PParentArea) {
        if(removed==removed->PParentArea->PLeftChild)removed->PParentArea->PLeftChild=child;
        else removed->PParentArea->PRightChild=child;
    } else {
        SetTreeRootNode(child);
        if(child)child->Colour=1;
    }
    colour=removed->Colour;
    if(removed!=area) {
        removed->Colour=area->Colour;
        removed->PParentArea=area->PParentArea;
        removed->PLeftChild=area->PLeftChild;
        removed->PRightChild=area->PRightChild;
        if(removed->PParentArea) {
            if(area==removed->PParentArea->PLeftChild)removed->PParentArea->PLeftChild=removed;
            else removed->PParentArea->PRightChild=removed;
        }
        if(removed->PLeftChild)removed->PLeftChild->PParentArea=removed;
        if(removed->PRightChild)removed->PRightChild->PParentArea=removed;
        if(GetTreeRoot()==area) {
            SetTreeRootNode(removed);
            removed->Colour=1;
        }
    }
    if(colour==1)RemoveFromTreeFixup(child,&info);
    area->PParentArea=0;
    area->PLeftChild=0;
    area->PRightChild=0;
}
