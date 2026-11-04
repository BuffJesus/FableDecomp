"""Fresh VC71 parity gate: CGenericVar default ctor (type only), copy ctor, destructor and scalar deleting destructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_7CGenericVar@@6B@':0x129c3e4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CGenericVar_Lifetime_009ea150.cpp'
ROOTS=[
 dict(name='ctor',source=F,symbol='??0CGenericVar@@QAE@XZ',address=0x9ea150,size=28,instructions=9,relocations=2),
 dict(name='dtor',source=F,symbol='??1CGenericVar@@UAE@XZ',address=0x9ea170,size=14,instructions=3,relocations=2),
 dict(name='scalar_deleting_dtor',source=F,symbol='??_GCGenericVar@@UAEPAXI@Z',address=0x9ea230,size=39,instructions=13,relocations=3),
 dict(name='copy_ctor',source=F,symbol='??0CGenericVar@@QAE@ABV0@@Z',address=0x9ea360,size=63,instructions=23,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-generic-var-lifetime.json',[],
  'CGenericVar default ctor (type only), copy ctor, destructor and scalar deleting destructor')
 print('NATIVE_GENERIC_VAR_LIFETIME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
