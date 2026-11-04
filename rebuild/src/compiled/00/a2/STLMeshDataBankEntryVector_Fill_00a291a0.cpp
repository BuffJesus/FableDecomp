// STLport fill<CMeshDataBankEntry**, CMeshDataBankEntry*>, retail 0x00A291A0: the
// out-of-line fastcall helper of vector<CMeshDataBankEntry*>::_M_fill_insert
// 0x00A2C0F0 (called at 0x00A2C184/0x00A2C1DE), reached from resize 0x00A2D0E0 in
// CMeshDataBank::NotifyBankSize 0x00A26660 (MeshesAndAnimations at +0x380).
#include <new>
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "engine/CMeshDataBank.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> __forceinline CMeshDataBankEntry** __copy_backward_ptrs(
    CMeshDataBankEntry** first,CMeshDataBankEntry** last,
    CMeshDataBankEntry** result,const __true_type&)
{
    const ptrdiff_t bytes=(const char*)last-(const char*)first;
    char* destination=(char*)result-bytes;
    if(bytes>0)memmove(destination,first,bytes);
    return (CMeshDataBankEntry**)destination;
}
}
template void _STL::vector<CMeshDataBankEntry*>::_M_fill_insert(CMeshDataBankEntry**,size_t,CMeshDataBankEntry* const&);
