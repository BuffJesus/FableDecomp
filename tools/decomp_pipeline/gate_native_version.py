"""Fresh VC71 parity gate for CVersion (ctor, operator==, GetAsString; 009EA080 TU)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??_C@_0BE@OEPPFPGL@?$CF02d?4?$CF02d?4?$CF02d?4?$CF04d?$AA@':0x129c3c8,  # "%02d.%02d.%02d.%04d"
 '___security_cookie':0x139c8a8,
 '@__security_check_cookie@4':0xbfe9f9,
 '_sprintf':0xbfeb10,
}
F=R/'rebuild/src/compiled/00/9e/CVersion_009ea080.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']  # GetAsString's char[256] carries a stack cookie
ROOTS=[
 dict(name='constructor',source=F,symbol='??0CVersion@@QAE@EEKK@Z',address=0x9ea080,size=32,instructions=10,relocations=0,flags=GS),
 dict(name='equal',source=F,symbol='??8CVersion@@QBE_NABV0@@Z',address=0x9ea0a0,size=47,instructions=17,relocations=0,flags=GS),
 dict(name='get_as_string',source=F,symbol='?GetAsString@CVersion@@QBE?AVCCharString@@XZ',address=0x9ea0d0,size=99,instructions=29,relocations=5,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-version.json',[],
  'Three genuine PDB-named CVersion roots (Ego_r parameter names day/month/year/build); operator== 009EA0A0 is absent from the manifest.')
 print('NATIVE_VERSION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
