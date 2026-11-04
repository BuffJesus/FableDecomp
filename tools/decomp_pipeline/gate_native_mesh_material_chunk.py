"""Fresh VC71 parity gate: C3DMeshFileMaterialChunk info/write/read/ResetParams/create/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteColour@CDataOutputStream@@QAEXABVCRGBColour@@@Z':0x993f30,
 '?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ':0x9945e0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '?ResetParams@C3DMeshFileMaterialChunk@@AAEXXZ':0xac0250,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_06GECNACHD@Name?3?5?$AA@':0x129e56c,  # derived from retail site; not in symbol_index
 '??_C@_04OOCFPGDI@MTRL?$AA@':0x129edc4,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileMaterialChunk@@6B@':0x129eee0,  # derived from retail site; not in symbol_index
 '??_C@_0BF@ODJILCFO@Material?5Description?$AA@':0x129ef0c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialChunk_Methods_00abfbf0.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileMaterialChunk@@UBE?BVCCharString@@XZ',address=0xabfbf0,size=79,instructions=24,relocations=6),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileMaterialChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabfcf0,size=127,instructions=49,relocations=10),
 dict(name='reset',source=F,symbol='?ResetParams@C3DMeshFileMaterialChunk@@AAEXXZ',address=0xac0250,size=89,instructions=29,relocations=0),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileMaterialChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac0fc0,size=84,instructions=33,relocations=6),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileMaterialChunk@@UBE?BVCCharString@@XZ',address=0xac1e70,size=25,instructions=9,relocations=2),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileMaterialChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xac0010,size=562,instructions=212,relocations=11),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-material-chunk.json',[],
  'C3DMeshFileMaterialChunk info/write/read/ResetParams/create/description')
 print('NATIVE_MESH_MATERIAL_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
