"""Fresh VC71 parity gate: CBasicString<char> CopyAndTerminate, Truncate (Align/AllocateDataFromStringLength/SetStringLength/ClearStringEnd expanded; operator new[]/delete[]), Erase, and the two filling constructors."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Truncate@?$CBasicString@D@@QAEXK@Z':0x9a0080,  # derived from retail site; not in symbol_index
 '?SetString@?$CBasicString@D@@IAEXPBDK@Z':0x9a0300,
 '?SetString@?$CBasicString@D@@IAEXKD@Z':0x9a0390,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '??_V@YAXPAX@Z':0xbfeb1c,
 '??_U@YAPAXI@Z':0xbfeb22,
}
F=R/'rebuild/src/compiled/00/9a/CBasicStringchar_Truncate_009a0080.cpp'
ROOTS=[
 dict(name='copy_and_terminate',source=F,symbol='?CopyAndTerminate@?$CBasicString@D@@KIXPADPBDK@Z',address=0x9a0040,size=39,instructions=19,relocations=0),
 dict(name='truncate',source=F,symbol='?Truncate@?$CBasicString@D@@QAEXK@Z',address=0x9a0080,size=166,instructions=65,relocations=2),
 dict(name='erase',source=F,symbol='?Erase@?$CBasicString@D@@QAEXKK@Z',address=0x9a0130,size=57,instructions=26,relocations=2),
 dict(name='ctor_fill',source=F,symbol='??0?$CBasicString@D@@QAE@KD@Z',address=0x9a0550,size=59,instructions=19,relocations=1),
 dict(name='ctor_text_length',source=F,symbol='??0?$CBasicString@D@@QAE@PBDK@Z',address=0x9a05e0,size=59,instructions=19,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-basic-string-char-edits.json',[],
  'CBasicString<char> CopyAndTerminate, Truncate (Align/AllocateDataFromStringLength/SetStringLength/ClearStringEnd expanded; operator new[]/delete[]), Erase, and the two filling constructors')
 print('NATIVE_BASIC_STRING_CHAR_EDITS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
