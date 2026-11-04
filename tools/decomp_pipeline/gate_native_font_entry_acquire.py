"""Fresh VC71 parity gate: CFontDataBankEntry::AcquireFont (lazy CStaticFontBank, counted copy out)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z':0x41947a,
 '??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z':0xab8e10,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a6/CFontDataBankEntry_AcquireFont_00a634f0.cpp'
ROOTS=[
 dict(name='acquire',source=F,symbol='?AcquireFont@CFontDataBankEntry@@QBE?AV?$CCountedPointer@VCFontBank@@@@XZ',address=0xa634f0,size=83,instructions=35,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-font-entry-acquire.json',[],
  'CFontDataBankEntry::AcquireFont (lazy CStaticFontBank, counted copy out)')
 print('NATIVE_FONT_ENTRY_ACQUIRE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
