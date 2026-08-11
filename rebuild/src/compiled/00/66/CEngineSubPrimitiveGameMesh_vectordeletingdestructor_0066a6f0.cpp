// CEngineSubPrimitiveGameMesh::`vector deleting destructor' 0x0066a6f0
#include <new>

struct CEngineSubPrimitiveGameMesh {
    void nonvirt_dtor();               // scalar destructor at 0x6661b0 (direct call)
    void free_this();                  // thiscall deallocation helper at 0x4c73b0
    void* vecDeletingDestructor(unsigned int flags);
};

void* CEngineSubPrimitiveGameMesh::vecDeletingDestructor(unsigned int flags)
{
    this->nonvirt_dtor();
    if (flags & 1)
        this->free_this();
    return this;
}