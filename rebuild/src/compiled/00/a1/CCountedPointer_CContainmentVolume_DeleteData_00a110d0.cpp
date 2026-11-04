// CCountedPointer<CContainmentVolume>::DeleteData (static __fastcall, retail 0x00A110D0):
// delete the volume through its virtual destructor. Its address is stored as the
// CCPPointerInfo delete function by CCountedPointer<CContainmentVolume>'s
// constructor (0x00A13380, `mov [eax+4],0xA110D0`), by the inlined construction in
// CPhysicsMeshObject::GetNavigationLevelVolumes (0x00A131B4) and by Reset (0x00A3168D);
// each pairs it with a `new CContainmentVolume` (constructor 0x00A752D0, vtable 0x0129DC94).
#include "engine/CContainmentVolume.h"
#include "fable_counted_pointer_inline.h"

template CCountedPointer<CContainmentVolume>::CCountedPointer(CContainmentVolume* data);
