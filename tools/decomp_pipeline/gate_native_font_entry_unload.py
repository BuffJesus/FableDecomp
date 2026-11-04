"""Fresh VC71 parity gate: CFontDataBankEntry Unload (inline ReleaseData), IsLoaded, ~CFontDataBankEntry and its scalar deleting destructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??1CFontDataBankEntry@@UAE@XZ':0xa63480,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_7CFontDataBankEntry@@6B@':0x129da70,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a6/CFontDataBankEntry_Unload_00a63580.cpp'
ROOTS=[
 dict(name='unload',source=F,symbol='?Unload@CFontDataBankEntry@@UAEXXZ',address=0xa63580,size=49,instructions=22,relocations=1),
 dict(name='dtor',source=F,symbol='??1CFontDataBankEntry@@UAE@XZ',address=0xa63480,size=100,instructions=39,relocations=4),
 dict(name='is_loaded',source=F,symbol='?IsLoaded@CFontDataBankEntry@@UBE_NXZ',address=0xa63550,size=11,instructions=5,relocations=0),
 dict(name='deleting_dtor',source=F,symbol='??_GCFontDataBankEntry@@UAEPAXI@Z',address=0xa63970,size=30,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-font-entry-unload.json',[],
  'CFontDataBankEntry Unload (inline ReleaseData), IsLoaded, ~CFontDataBankEntry and its scalar deleting destructor')
 print('NATIVE_FONT_ENTRY_UNLOAD_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
