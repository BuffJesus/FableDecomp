"""Fresh VC71 parity gate: C3DMeshFilePartitionChunk::CreateNewInstance."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04ODFMPJDG@MPRT?$AA@':0x129ef40,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFilePartitionChunk@@6B@':0x129ef4c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ac/C3DMeshFilePartitionChunk_Create_00ac29c0.cpp'
ROOTS=[
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFilePartitionChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac29c0,size=76,instructions=30,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-partition-chunk.json',[],
  'C3DMeshFilePartitionChunk::CreateNewInstance')
 print('NATIVE_MESH_PARTITION_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
