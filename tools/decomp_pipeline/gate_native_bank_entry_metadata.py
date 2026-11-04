"""Fresh VC71 parity gate for CBankFile::GetEntryDataOffset 009CC270 and ReadData 009CBF60 and CountEntries 009CC3B0 (EntryExists TU)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# CPackedUIntArray::operator[] is the landed provider rebuild/src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp.
ADDRESS={'??ACPackedUIntArray@@QBEKK@Z':0xa629f0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??0CDateAndTime@@QAE@JKKKKKK@Z':0x9e9a70,'?GetTotalDataSize@CBankFile@@QBEKXZ':0x9cc450}
ROOTS=[
 dict(name='entry_data_offset',source=R/'rebuild/src/compiled/00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp',symbol='?GetEntryDataOffset@CBankFile@@QBEKK@Z',address=0x9cc270,size=36,instructions=12,relocations=1),
 dict(name='read_data',source=R/'rebuild/src/compiled/00/9c/CBankFile_ReadData_009cbf60.cpp',symbol='?ReadData@CBankFile@@IBEXKPAXK@Z',address=0x9cbf60,size=40,instructions=17,relocations=0),
 dict(name='count_entries',source=R/'rebuild/src/compiled/00/9c/CBankFile_EntryExists_009cc190.cpp',symbol='?CountEntries@CBankFile@@QBEKXZ',address=0x9cc3b0,size=83,instructions=41,relocations=1),
 dict(name='get_bank_handle',source=R/'rebuild/src/compiled/00/9c/CBankFile_FileAccessors_009cba40.cpp',symbol='?GetBankHandle@CBankFile@@QBE?AVCCharString@@XZ',address=0x9cba40,size=25,instructions=9,relocations=1),
 dict(name='get_bank_pathname',source=R/'rebuild/src/compiled/00/9c/CBankFile_FileAccessors_009cba40.cpp',symbol='?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ',address=0x9cbf10,size=20,instructions=9,relocations=0),
 dict(name='make_writable',source=R/'rebuild/src/compiled/00/9c/CBankFile_FileAccessors_009cba40.cpp',symbol='?MakeWritable@CBankFile@@AAEXXZ',address=0x9cbf90,size=70,instructions=28,relocations=0),
 dict(name='game_text_last_modified',source=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_GetLastModifiedTime_009cb710.cpp',symbol='?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z',address=0x9cb710,size=32,instructions=14,relocations=1),
 dict(name='get_unused_space',source=R/'rebuild/src/compiled/00/9c/CBankFile_GetUnusedSpace_009cc4c0.cpp',symbol='?GetUnusedSpace@CBankFile@@QBEKXZ',address=0x9cc4c0,size=38,instructions=14,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-entry-metadata.json',
  ['work/native-recreation/rendering/009cc270/20261015/native-first (262,144 cases / 1,835,008 snapshots)'],
  'Two genuine CBankFile metadata roots, byte-identical to retail after audited relocation patching. Virtual disk SetPosition/Read and packed lookup are separate providers.')
 print('NATIVE_BANK_ENTRY_METADATA_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
