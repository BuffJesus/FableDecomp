"""Fresh VC71 parity gate: GFSetSystemStartTime (performance counter, else timeGetTime)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableTimerStartTick_013CB2F0@@3_JA':0x13cb2f0,
 '__imp__QueryPerformanceCounter@4':0x143fe00,  # derived from retail site; not in symbol_index
 '__imp__timeGetTime@0':0x14404b4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/Global_GFSetSystemStartTime_009e1cb0.cpp'
ROOTS=[
 dict(name='start_time',source=F,symbol='?GFSetSystemStartTime@@YIXXZ',address=0x9e1cb0,size=79,instructions=24,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-system-start-time.json',[],
  'GFSetSystemStartTime (performance counter, else timeGetTime)')
 print('NATIVE_GLOBAL_SYSTEM_START_TIME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
