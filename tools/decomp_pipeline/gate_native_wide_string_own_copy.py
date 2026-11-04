"""Fresh VC71 parity gate: CWideString copy-on-write helpers (MakeOwnCopyOfData, MakeOwnCopyOfDataOrEmptyString; AllocStringData and UnassignString expanded) and SetAsSubString."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z':0x99b3c0,
 '?UnassignString@CWideString@@AAEXXZ':0x99b4d0,
 '??$_M_range_initialize@PBG@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEXPBG0ABUforward_iterator_tag@1@@Z':0x99c670,
 '??0?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAE@IGABV?$allocator@G@1@@Z':0x99ca70,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CWideString_MakeOwnCopyOfData_0099b560.cpp'
ROOTS=[
 dict(name='own_copy',source=F,symbol='?MakeOwnCopyOfData@CWideString@@AAEXXZ',address=0x99b560,size=128,instructions=51,relocations=4),
 dict(name='own_copy_or_empty',source=F,symbol='?MakeOwnCopyOfDataOrEmptyString@CWideString@@AAEXXZ',address=0x99b5e0,size=187,instructions=75,relocations=6),
 dict(name='set_as_substring',source=F,symbol='?SetAsSubString@CWideString@@QAEXPBGJ@Z',address=0x99b770,size=93,instructions=39,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-own-copy.json',[],
  'CWideString copy-on-write helpers (MakeOwnCopyOfData, MakeOwnCopyOfDataOrEmptyString; AllocStringData and UnassignString expanded) and SetAsSubString')
 print('NATIVE_WIDE_STRING_OWN_COPY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
