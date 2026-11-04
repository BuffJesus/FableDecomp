"""Fresh VC71 parity gate: CStringParser comment queries: IsPosInComments, GetNextCommentStart, GetNextCommentEnd (STLport list walk)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/9b/CStringParser_Comments_009ba830.cpp'
ROOTS=[
 dict(name='is_pos_in_comments',source=F,symbol='?IsPosInComments@CStringParser@@IAE_NPBD@Z',address=0x9ba830,size=42,instructions=17,relocations=0),
 dict(name='next_comment_start',source=F,symbol='?GetNextCommentStart@CStringParser@@IAEPBDPBD@Z',address=0x9ba860,size=38,instructions=15,relocations=0),
 dict(name='next_comment_end',source=F,symbol='?GetNextCommentEnd@CStringParser@@IAEPBDPBD@Z',address=0x9ba890,size=38,instructions=15,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-parser-comments.json',[],
  'CStringParser comment queries: IsPosInComments, GetNextCommentStart, GetNextCommentEnd (STLport list walk)')
 print('NATIVE_STRING_PARSER_COMMENTS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
