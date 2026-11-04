"""Fresh VC71 parity gate: CFontDataBank::LoadFont (new CStaticFontBank(this, index))."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z':0xab8e10,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a6/CFontDataBank_LoadFont_00a63350.cpp'
ROOTS=[
 dict(name='load_font',source=F,symbol='?LoadFont@CFontDataBank@@QAEPAVCFontBank@@K@Z',address=0xa63350,size=43,instructions=17,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-font-bank-load-font.json',[],
  'CFontDataBank::LoadFont (new CStaticFontBank(this, index))')
 print('NATIVE_FONT_BANK_LOAD_FONT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
