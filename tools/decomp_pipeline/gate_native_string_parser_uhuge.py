"""Fresh VC71 parity gate: CStringParser::ReadAsUHUGE (64-bit decimal via __allmul; MoveStringPos expanded with CalcInQuotes from the same unit)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z':0x9b9570,
 '?SkipWhitespace@CStringParser@@QAEJXZ':0x9b9c60,
 '__allmul':0xbfeb50,  # derived from retail site; not in symbol_index
 '_isdigit':0xbfeda6,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9b/CStringParser_ReadAsUHUGE_009ba5b0.cpp'
ROOTS=[
 dict(name='read_uhuge',source=F,symbol='?ReadAsUHUGE@CStringParser@@QAE_KXZ',address=0x9ba5b0,size=155,instructions=68,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-parser-uhuge.json',[],
  'CStringParser::ReadAsUHUGE (64-bit decimal via __allmul; MoveStringPos expanded with CalcInQuotes from the same unit)')
 print('NATIVE_STRING_PARSER_UHUGE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
