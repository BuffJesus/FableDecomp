// STLport __uninitialized_copy<CCountedPointer<CMeshDataBankEntry>*, CCountedPointer<CMeshDataBankEntry>*>,
// retail 0x00A2A610: the out-of-line fastcall range copy of
// CMeshDataBank::DynamicInfoBuffer (CArray<CCountedPointer<CMeshDataBankEntry> >, retail +0x3B0).
// Callers: vector::_M_insert_overflow 0x00A2D4D0 (0x00A2D51D, 0x00A2D572) and
// vector::_M_fill_insert 0x00A2D960 (0x00A2D9C7, 0x00A2DA24), reached through
// resize 0x00A2DBC0 from CMeshDataBank::NotifyEntryAdded (0x00A26F48, this+0x3B0).
// The element copy is CCountedPointer's copy constructor, expanded inline.
#include <new>
#include "engine/CMeshDataBankEntry.h"
#include "fable_counted_pointer_inline.h"
#include "fable_stlport_uninitialized.h"

template CCountedPointer<CMeshDataBankEntry>* FABLE_FASTCALL _STL::__uninitialized_copy(
    CCountedPointer<CMeshDataBankEntry>*, CCountedPointer<CMeshDataBankEntry>*,
    CCountedPointer<CMeshDataBankEntry>*, const _STL::__false_type&);
