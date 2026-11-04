"""Fresh VC71 parity gate: CStringParser::ReadNextItemAsQuotedString (escape-aware close-quote scan; \\ to one backslash, other backslashes dropped; position update with CalcInQuotes expanded)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??YCCharString@@QAEABV0@D@Z':0x99f180,
 '?SkipWhitespace@CStringParser@@QAEJXZ':0x9b9c60,
}
F=R/'rebuild/src/compiled/00/9b/CStringParser_ReadNextItemAsQuotedString_009ba1d0.cpp'
ROOTS=[
 dict(name='read_quoted',source=F,symbol='?ReadNextItemAsQuotedString@CStringParser@@QAE_NAAVCCharString@@@Z',address=0x9ba1d0,size=255,instructions=109,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-parser-quoted.json',[],
  'CStringParser::ReadNextItemAsQuotedString (escape-aware close-quote scan; \\ to one backslash, other backslashes dropped; position update with CalcInQuotes expanded)')
 print('NATIVE_STRING_PARSER_QUOTED_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
