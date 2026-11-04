"""Fresh VC71 parity gate: CCharStringData::ReadFromFile (read into the existing buffer when the length is positive)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/99/CCharStringData_ReadFromFile_0099e4d0.cpp'
ROOTS=[
 dict(name='read_from_file',source=F,symbol='?ReadFromFile@CCharStringData@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z',address=0x99e4d0,size=35,instructions=17,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-data-read-file.json',[],
  'CCharStringData::ReadFromFile (read into the existing buffer when the length is positive)')
 print('NATIVE_STRING_DATA_READ_FILE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
