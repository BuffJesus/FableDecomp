"""Fresh VC71 parity gate: CSystemManager::ExceptionHandler (signed switch over exception codes, sprintf into the shared error text; embedded jump table)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?IsValid@CInitBaseClass@@QBE_NXZ':0x99a330,
 '$L3472':0x9a50ed,  # derived from retail site; not in symbol_index
 '$L3470':0x9a5162,  # derived from retail site; not in symbol_index
 '$L3473':0x9a517a,  # derived from retail site; not in symbol_index
 '$L3475':0x9a5192,  # derived from retail site; not in symbol_index
 '$L3477':0x9a51aa,  # derived from retail site; not in symbol_index
 '$L3479':0x9a51c2,  # derived from retail site; not in symbol_index
 '$L3481':0x9a51da,  # derived from retail site; not in symbol_index
 '$L3625':0x9a51f4,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_0EG@MAGEJCEI@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129ab98,  # derived from retail site; not in symbol_index
 '??_C@_0DC@FELBLHBF@CRITICAL?5ERROR?5?9?5An?5integer?5oper@':0x129abe0,  # derived from retail site; not in symbol_index
 '??_C@_0EC@DGPFOLMO@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129ac18,  # derived from retail site; not in symbol_index
 '??_C@_0GA@JMGHDCFK@CRITICAL?5ERROR?5?9?5The?5exponent?5of@':0x129ac60,  # derived from retail site; not in symbol_index
 '??_C@_0FB@FCHDJIPE@CRITICAL?5ERROR?5?9?5A?5floating?5poin@':0x129acc0,  # derived from retail site; not in symbol_index
 '??_C@_0EM@FJOBBFBO@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129ad18,  # derived from retail site; not in symbol_index
 '??_C@_0FB@PLNLOCMO@CRITICAL?5ERROR?5?9?5The?5result?5of?5a@':0x129ad68,  # derived from retail site; not in symbol_index
 '??_C@_0FB@LLJDCDOJ@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129adc0,  # derived from retail site; not in symbol_index
 '??_C@_0FL@JCBDLOBP@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129ae18,  # derived from retail site; not in symbol_index
 '??_C@_0EI@CEMOLGPK@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129ae78,  # derived from retail site; not in symbol_index
 '??_C@_0HC@LNFAIEFK@CRITICAL?5ERROR?5?9?5The?5program?5att@':0x129aec0,  # derived from retail site; not in symbol_index
 '??_C@_0DJ@HILGMFGK@STUPID?5ERROR?5?9?5The?5program?5has?5e@':0x129af34,  # derived from retail site; not in symbol_index
 '??_C@_0EB@MFIIBBBL@STUPID?5ERROR?5?9?5The?5program?5has?5e@':0x129af70,  # derived from retail site; not in symbol_index
 '??_C@_0DC@CPHHNDIP@CRITICAL?5ERROR?5?9?5An?5access?5viola@':0x129afb4,  # derived from retail site; not in symbol_index
 '?g_FableExceptionText_013CB0C8@@3PADA':0x13cb0c8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9a/CSystemManager_ExceptionHandler_009a5020.cpp'
ROOTS=[
 dict(name='exception_handler',source=F,symbol='?ExceptionHandler@CSystemManager@@QAEHH@Z',address=0x9a5020,size=496,instructions=129,code_size=0x1d4,relocations=51),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-system-manager-exception-handler.json',[],
  'CSystemManager::ExceptionHandler (signed switch over exception codes, sprintf into the shared error text; embedded jump table)')
 print('NATIVE_SYSTEM_MANAGER_EXCEPTION_HANDLER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
