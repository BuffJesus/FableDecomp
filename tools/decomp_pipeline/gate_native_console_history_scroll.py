"""Fresh VC71 parity gate: CConsole::ScrollUpThroughHistory / ScrollDownThroughHistory (visible history window moves; ScrollToHistoryEnd in the same TU is a 4-line near match)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/9e/CConsole_HistoryScroll_009eabe0.cpp'
ROOTS=[
 dict(name='scroll_up',source=F,symbol='?ScrollUpThroughHistory@CConsole@@AAEXXZ',address=0x9eabe0,size=70,instructions=32,relocations=0),
 dict(name='scroll_down',source=F,symbol='?ScrollDownThroughHistory@CConsole@@AAEXXZ',address=0x9eac30,size=73,instructions=33,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-history-scroll.json',[],
  'CConsole::ScrollUpThroughHistory / ScrollDownThroughHistory (visible history window moves; ScrollToHistoryEnd in the same TU is a 4-line near match)')
 print('NATIVE_CONSOLE_HISTORY_SCROLL_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
