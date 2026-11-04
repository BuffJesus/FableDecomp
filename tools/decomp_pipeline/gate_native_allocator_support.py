"""Fresh VC71 parity gate: CMemoryAllocator support members (debug headers/records, tracking ID, own storage, CRT wrappers, synchronisation, size-class peeks)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,
 '_strncpy':0xbfe9c2,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '__stricmp':0xbfeba8,  # derived from retail site; not in symbol_index
 '__splitpath':0xbfed52,  # derived from retail site; not in symbol_index
 '_calloc':0xbfedd6,  # derived from retail site; not in symbol_index
 '_realloc':0xbfeddc,  # derived from retail site; not in symbol_index
 '??_C@_07NBCGADJA@Unknown?$AA@':0x122e5a4,
 '?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A':0x1396868,
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70,
 '?g_FableAllocatorRangeStart_013CAAA4@@3KA':0x13caaa4,
 '?g_FableAllocatorRangeEnd_013CAAA8@@3KA':0x13caaa8,
 '__imp__VirtualFree@12':0x143fd40,  # derived from retail site; not in symbol_index
 '__imp__VirtualAlloc@16':0x143fd44,  # derived from retail site; not in symbol_index
 '__imp__DeleteCriticalSection@4':0x143fde4,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
 '__imp__InitializeCriticalSection@4':0x143fee4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9f/CMemoryAllocator_OutOfLineSupport_009f1f90.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='alloc_id_from_pmem',source=F,symbol='?GetAllocIDFromPMem@CMemoryAllocator@@IAEEPAX@Z',address=0x9f1e80,size=36,instructions=11,relocations=2,flags=GS),
 dict(name='pmem_from_debug',source=F,symbol='?GetPMemFromPMemIncDebugHeader@CMemoryAllocator@@IAEPAXPAX@Z',address=0x9f1eb0,size=10,instructions=3,relocations=0,flags=GS),
 dict(name='debug_from_pmem',source=F,symbol='?GetPMemIncDebugHeaderFromPMem@CMemoryAllocator@@IAEPAXPAX@Z',address=0x9f1ec0,size=10,instructions=3,relocations=0,flags=GS),
 dict(name='init_debug_header',source=F,symbol='?InitDebugHeader@CMemoryAllocator@@IAEXAAVCDebugHeader@1@@Z',address=0x9f1ed0,size=13,instructions=3,relocations=0,flags=GS),
 dict(name='init_debug_record',source=F,symbol='?InitDebugRecord@CMemoryAllocator@@IAEXAAVCDebugRecord@1@J@Z',address=0x9f1ee0,size=30,instructions=7,relocations=0,flags=GS),
 dict(name='set_tracking_id',source=F,symbol='?SetCurrentTrackingID@CMemoryAllocator@@QAEXK@Z',address=0x9f1f20,size=13,instructions=3,relocations=0,flags=GS),
 dict(name='get_tracking_id',source=F,symbol='?GetCurrentTrackingID@CMemoryAllocator@@QAEKXZ',address=0x9f1f30,size=8,instructions=2,relocations=0,flags=GS),
 dict(name='internal_validate',source=F,symbol='?InternalValidate@CMemoryAllocator@@QAEXXZ',address=0x9f1f50,size=1,instructions=1,relocations=0,flags=GS),
 dict(name='get_debug_header',source=F,symbol='?GetDebugHeader@CMemoryAllocator@@IAEAAVCDebugHeader@1@PAX@Z',address=0x9f1f60,size=10,instructions=3,relocations=0,flags=GS),
 dict(name='operator_new',source=F,symbol='??2CMemoryAllocator@@KIPAXI@Z',address=0x9f1f70,size=17,instructions=6,relocations=1,flags=GS),
 dict(name='operator_delete',source=F,symbol='??3CMemoryAllocator@@KIXPAX@Z',address=0x9f1f90,size=15,instructions=5,relocations=1,flags=GS),
 dict(name='gf_calloc',source=F,symbol='?GFCalloc@@YIPAXII@Z',address=0x9f1fa0,size=11,instructions=5,relocations=1,flags=GS),
 dict(name='gf_malloc',source=F,symbol='?GFMalloc@@YIPAXI@Z',address=0x9f1fb0,size=10,instructions=4,relocations=1,flags=GS),
 dict(name='gf_free',source=F,symbol='?GFFree@@YIXPAX@Z',address=0x9f1fc0,size=8,instructions=4,relocations=1,flags=GS),
 dict(name='gf_realloc',source=F,symbol='?GFRealloc@@YIPAXPAXI@Z',address=0x9f1fd0,size=11,instructions=5,relocations=1,flags=GS),
 dict(name='init_sync',source=F,symbol='?InitSynchronisation@CMemoryAllocator@@KIXXZ',address=0x9f1fe0,size=12,instructions=3,relocations=2,flags=GS),
 dict(name='uninit_sync',source=F,symbol='?UninitSynchronisation@CMemoryAllocator@@KIXXZ',address=0x9f1ff0,size=12,instructions=3,relocations=2,flags=GS),
 dict(name='enter_sync',source=F,symbol='?EnterSynchronisation@CMemoryAllocator@@KIXXZ',address=0x9f2000,size=12,instructions=3,relocations=2,flags=GS),
 dict(name='leave_sync',source=F,symbol='?LeaveSynchronisation@CMemoryAllocator@@KIXXZ',address=0x9f2010,size=12,instructions=3,relocations=2,flags=GS),
 dict(name='peek_controlled',source=F,symbol='?PeekAllocatorMemoryControlled@CMemoryAllocator@@QBEJJ@Z',address=0x9f2020,size=21,instructions=6,relocations=1,flags=GS),
 dict(name='peek_allocated',source=F,symbol='?PeekAllocatorMemoryAllocated@CMemoryAllocator@@QBEJJ@Z',address=0x9f2040,size=21,instructions=6,relocations=1,flags=GS),
 dict(name='is_fixed_size',source=F,symbol='?IsAllocatorFixedSize@CMemoryAllocator@@QBE_NK@Z',address=0x9f2060,size=25,instructions=8,relocations=1,flags=GS),
 dict(name='save_tracker_log',source=F,symbol='?SaveStackMemoryTrackerLog@CMemoryAllocator@@QAEXPBD@Z',address=0x9f2080,size=3,instructions=1,relocations=0,flags=GS),
 dict(name='tracker_start',source=F,symbol='?StackMemoryTrackerStartAutosavingMaxMemory@CMemoryAllocator@@QAEXPBD@Z',address=0x9f2090,size=3,instructions=1,relocations=0,flags=GS),
 dict(name='tracker_stop',source=F,symbol='?StackMemoryTrackerStopAutosavingMaxMemory@CMemoryAllocator@@QAEXXZ',address=0x9f20a0,size=1,instructions=1,relocations=0,flags=GS),
 dict(name='get_debug_record',source=F,symbol='?GetDebugRecord@CMemoryAllocator@@IAEAAVCDebugRecord@1@PAX@Z',address=0x9f2830,size=31,instructions=13,relocations=2,flags=GS),
 dict(name='get_tracking_debug_record',source=F,symbol='?GetTrackingDebugRecord@CMemoryAllocator@@IAEAAVCTrackingDebugRecord@1@PAX@Z',address=0x9f2850,size=19,instructions=7,relocations=1,flags=GS),
 dict(name='check_checksums',source=F,symbol='?CheckChecksums@CMemoryAllocator@@IAE_NPAX@Z',address=0x9f29c0,size=56,instructions=23,relocations=2,flags=GS),
 dict(name='init_tracking_debug_record',source=F,symbol='?InitTrackingDebugRecord@CMemoryAllocator@@IAEXAAVCTrackingDebugRecord@1@PBDJ1J@Z',address=0x9f26f0,size=235,instructions=73,relocations=7,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-allocator-support.json',[],
  'CMemoryAllocator support members (debug headers/records, tracking ID, own storage, CRT wrappers, synchronisation, size-class peeks)')
 print('NATIVE_ALLOCATOR_SUPPORT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
