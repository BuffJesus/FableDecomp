"""Fresh VC71 parity gate: CHandleBasedMemoryPool growable test, critical section, Uninitialise/Initialise, shutdown, ResizeArea/CombineAdjacentAreas, Validate, ResizePool."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetTime@@YINXZ':0x9e1bc0,
 '?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,
 '??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90,
 '?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,
 '?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,
 '??1CMemoryAllocator@@IAE@XZ':0x9f2310,
 '?Create@CMemoryAllocator@@KIXXZ':0x9f2870,
 '?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,
 '?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z':0x9f2a40,
 '?InsertFreeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@@Z':0xa01da0,
 '?CombineAreas@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z':0xa01e40,
 '?Uninitialise@CHandleBasedMemoryPool@@QAEXXZ':0xa02190,
 '?BeginShutdown@CHandleBasedMemoryPool@@QAE_NM@Z':0xa02220,  # derived from retail site; not in symbol_index
 '?EndShutdown@CHandleBasedMemoryPool@@QAEXXZ':0xa02370,  # derived from retail site; not in symbol_index
 '?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,
 '?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,
 '__imp__TryEnterCriticalSection@4':0x143fd30,  # derived from retail site; not in symbol_index
 '__imp__DeleteCriticalSection@4':0x143fde4,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
 '__imp__Sleep@4':0x143fe1c,  # derived from retail site; not in symbol_index
 '__imp__InitializeCriticalSection@4':0x143fee4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='test_if_area_is_growable',source=F,symbol='?TestIfAreaIsGrowable@CHandleBasedMemoryPool@@AAE_NPAVCHeader@1@J@Z',address=0xa02020,size=209,instructions=95,relocations=0),
 dict(name='clear_free_lists',source=F,symbol='?ClearFreeLists@CHandleBasedMemoryPool@@AAEXXZ',address=0xa02100,size=17,instructions=8,relocations=0),
 dict(name='begin_critical',source=F,symbol='?BeginCritical@CHandleBasedMemoryPool@@AAE_N_N@Z',address=0xa02140,size=48,instructions=16,relocations=2),
 dict(name='end_critical',source=F,symbol='?EndCritical@CHandleBasedMemoryPool@@AAEXXZ',address=0xa02170,size=21,instructions=7,relocations=1),
 dict(name='uninitialise',source=F,symbol='?Uninitialise@CHandleBasedMemoryPool@@QAEXXZ',address=0xa02190,size=143,instructions=48,relocations=12),
 dict(name='begin_shutdown',source=F,symbol='?BeginShutdown@CHandleBasedMemoryPool@@QAE_NM@Z',address=0xa02220,size=334,instructions=111,relocations=18),
 dict(name='end_shutdown',source=F,symbol='?EndShutdown@CHandleBasedMemoryPool@@QAEXXZ',address=0xa02370,size=159,instructions=57,relocations=9),
 dict(name='resize_area',source=F,symbol='?ResizeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@J@Z',address=0xa02410,size=92,instructions=36,relocations=1),
 dict(name='combine_adjacent_areas',source=F,symbol='?CombineAdjacentAreas@CHandleBasedMemoryPool@@AAEXAAPAVCHeader@1@@Z',address=0xa02470,size=65,instructions=30,relocations=2),
 dict(name='validate',source=F,symbol='?Validate@CHandleBasedMemoryPool@@QAEXXZ',address=0xa024c0,size=161,instructions=63,relocations=2),
 dict(name='initialise',source=F,symbol='?Initialise@CHandleBasedMemoryPool@@QAEXK_NK@Z',address=0xa02570,size=151,instructions=52,relocations=9),
 dict(name='resize_pool',source=F,symbol='?ResizePool@CHandleBasedMemoryPool@@QAE_NKM@Z',address=0xa02680,size=43,instructions=17,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-critical.json',[],
  'CHandleBasedMemoryPool growable test, critical section, Uninitialise/Initialise, shutdown, ResizeArea/CombineAdjacentAreas, Validate, ResizePool')
 print('NATIVE_POOL_CRITICAL_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
