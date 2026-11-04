"""Fresh VC71 parity gate: CMemoryAllocator statistics (controlled, fixed-size, free areas, fragmentation, header overhead, per-ID usage, Validate) and Destroy."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CMemoryAllocator@@IAE@XZ':0x9f2310,
 '?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A':0x1396868,
 '?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70,
 '?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,
 '?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,
 '__imp__VirtualFree@12':0x143fd40,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9f/CMemoryAllocator_Statistics_009f2400.cpp'
ROOTS=[
 dict(name='total_controlled',source=F,symbol='?GetTotalMemoryControlledByAllocator@CMemoryAllocator@@QAEKXZ',address=0x9f2400,size=77,instructions=30,relocations=5),
 dict(name='total_fixed_size',source=F,symbol='?GetTotalFixedSizeMemoryAllocated@CMemoryAllocator@@QAEKXZ',address=0x9f24a0,size=83,instructions=32,relocations=5),
 dict(name='no_free_areas',source=F,symbol='?GetNoFreeAreas@CMemoryAllocator@@QAEKXZ',address=0x9f2500,size=77,instructions=30,relocations=5),
 dict(name='fragmentation',source=F,symbol='?GetFragmentation@CMemoryAllocator@@QAEMXZ',address=0x9f25a0,size=97,instructions=32,relocations=5),
 dict(name='header_overhead',source=F,symbol='?GetHeaderOverhead@CMemoryAllocator@@QAEKXZ',address=0x9f2610,size=77,instructions=30,relocations=5),
 dict(name='used_with_tracking_id',source=F,symbol='?GetTotalMemoryUsedWithTrackingID@CMemoryAllocator@@QAEKE@Z',address=0x9f27e0,size=39,instructions=11,relocations=4),
 dict(name='validate',source=F,symbol='?Validate@CMemoryAllocator@@QAEXXZ',address=0x9f2810,size=23,instructions=5,relocations=4),
 dict(name='destroy',source=F,symbol='?Destroy@CMemoryAllocator@@KIXXZ',address=0x9f2a00,size=61,instructions=17,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-allocator-statistics.json',[],
  'CMemoryAllocator statistics (controlled, fixed-size, free areas, fragmentation, header overhead, per-ID usage, Validate) and Destroy')
 print('NATIVE_ALLOCATOR_STATISTICS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
