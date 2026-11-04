// _STL::__uninitialized_copy<pair<long, CCountedPointer<CContainmentVolume> >*, ...>
// (first, last, result, __false_type const&) @ 0x00A13580: copy-construct the
// CVectorMap<long, CCountedPointer<CContainmentVolume> > entries (12 bytes: key,
// Data, Info with ++Info->RefCount) into raw storage. Its two callers are the
// vector::_M_insert_overflow at 0x00A139B0 (0x00A13A09 old prefix, 0x00A13A64
// suffix), which CPhysicsMeshObject::GetNavigationLevelVolumes (0x00A13100) reaches
// after `new CContainmentVolume` + CCountedPointer (DeleteData 0x00A110D0).
#include "engine/CContainmentVolume.h"
#include "fable_counted_pointer_inline.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_tree.h"

#include <new>

typedef _STL::pair<long, CCountedPointer<CContainmentVolume> > ContainmentVolumeEntry;
template ContainmentVolumeEntry* FABLE_FASTCALL _STL::__uninitialized_copy(ContainmentVolumeEntry* __first,
                                                                           ContainmentVolumeEntry* __last,
                                                                           ContainmentVolumeEntry* __result,
                                                                           const _STL::__false_type&);
