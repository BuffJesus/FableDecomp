"""Fresh VC71 parity gate: CWideString Contains (wcsstr), <=/>/>= (wcscmp), SaveToFile (no terminator), !=(wchar_t), GetCRC."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FableCRC_Calc_004014A0@@YIIIPBXI@Z':0x4014a0,
 '_wcscmp':0xbfe94c,  # derived from retail site; not in symbol_index
 '_wcsstr':0xbfeb2e,  # derived from retail site; not in symbol_index
 '_wcslen':0xbfed94,  # derived from retail site; not in symbol_index
 '?g_FableEmptyWideString_0129A8E0@@3PAGA':0x129a8e0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CWideString_Compare_0099af20.cpp'
ROOTS=[
 dict(name='contains',source=F,symbol='?Contains@CWideString@@QBE_NABV1@@Z',address=0x99af20,size=70,instructions=28,relocations=4),
 dict(name='less_equal',source=F,symbol='??NCWideString@@QBE_NABV0@@Z',address=0x99afb0,size=56,instructions=22,relocations=3),
 dict(name='greater',source=F,symbol='??OCWideString@@QBE_NABV0@@Z',address=0x99aff0,size=56,instructions=22,relocations=3),
 dict(name='greater_equal',source=F,symbol='??PCWideString@@QBE_NABV0@@Z',address=0x99b030,size=56,instructions=22,relocations=3),
 dict(name='save_to_file',source=F,symbol='?SaveToFile@CWideString@@QAEXAAVCAFile@@@Z',address=0x99b0c0,size=43,instructions=22,relocations=1),
 dict(name='not_equal_char',source=F,symbol='??9CWideString@@QBE_NG@Z',address=0x99b1d0,size=71,instructions=26,relocations=1),
 dict(name='crc',source=F,symbol='?GetCRC@CWideString@@QBEKXZ',address=0x99b260,size=54,instructions=24,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-compare.json',[],
  'CWideString Contains (wcsstr), <=/>/>= (wcscmp), SaveToFile (no terminator), !=(wchar_t), GetCRC')
 print('NATIVE_WIDE_STRING_COMPARE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
