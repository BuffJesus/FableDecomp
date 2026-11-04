"""Fresh VC71 parity gate for the CBankFileAsync open path (009D5820)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# Callee identities agree with rebuild/manifest/functions.tsv names.
ADDRESS={
 '?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z':0x9d1090,
 '??2@YAPAXI@Z':0xbfea1a,
 '??0CThreadedFile@@QAE@XZ':0x98dfd0,
 '?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z':0x9a9c80,
 '?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ':0x9cbf10,
 '?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z':0x98e1e0,
 '??1CWideString@@QAE@XZ':0x99b510,
}
ROOTS=[dict(name='open_file_read_only',source=R/'rebuild/src/compiled/00/9d/CBankFileAsync_OpenFileReadOnly_009d5820.cpp',symbol='?OpenFileReadOnly@CBankFileAsync@@UAE_NABVCWideString@@K@Z',address=0x9d5820,size=122,instructions=44,relocations=7)]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-async-open.json',[],
  'Genuine virtual override, byte-identical to retail after audited relocation patching. Base open, threaded-file construction/open and pathname are separate providers.')
 print('NATIVE_BANK_ASYNC_OPEN_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
